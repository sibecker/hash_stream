//-------------------------------- traits.h ------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_TRAITS
#define HASH_TRAITS

#include "endian.h"
#include <cstddef>
#include <cstring>      // memmove
#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

// Everything in namespace xstd, excluding those items in xstd::detail,
//  is proposed.

// C++14 is assumed below because std::index_sequence_for makes hash_append
// 	for tuple just so easy.  So in for a penny, in for a pound...

namespace xstd
{

namespace detail
{
// SFINAE test classes to see if endian T::endian exists
template<typename T>
class has_endian {
    typedef char one;
    struct two {
        char x[2];
    };

    template<typename C>
    static constexpr one test(decltype(C::endian));

    template<typename C>
    static constexpr two test(...);

public:
    static inline constexpr bool value = sizeof(test<T>(endian::native)) == sizeof(char);
};

} // detail

// byte_order<HashAlgorithm>

// Defaults to endian::native but uses HashAlgorithm::endian if supplied

template <typename HashAlgorithm, bool HasEndian = detail::has_endian<HashAlgorithm>::value>
struct byte_order;

template <typename HashAlgorithm>
struct byte_order<HashAlgorithm, true>
        : public std::integral_constant<endian, HashAlgorithm::endian>
{};

template <typename HashAlgorithm>
struct byte_order<HashAlgorithm, false>
        : public std::integral_constant<endian, endian::native>
{};

template <typename HashAlgorithm>
inline constexpr endian byte_order_v = byte_order<HashAlgorithm>::value;

namespace detail
{

//  Standards-worthy utilities, but not for this proposal...

template <class T>
constexpr
inline
void
reverse_bytes(T& t)
{
    unsigned char* bytes = static_cast<unsigned char*>(std::memmove(std::addressof(t),
                                                                    std::addressof(t),
                                                                    sizeof(T)));
    for (unsigned i = 0; i < sizeof(T)/2; ++i)
        std::swap(bytes[i], bytes[sizeof(T)-1-i]);
}

template <class T, class Hasher>
constexpr
inline
void
maybe_reverse_bytes(T& t, Hasher&)
{
    if constexpr (byte_order_v<Hasher> != endian::native)
        reverse_bytes(t);
}

}  // detail

// is_uniquely_represented<T>

// A type T is contiguously hashable if for all combinations of two values of
// 	a type, say x and y, if x == y, then it must also be true that
// 	memcmp(addressof(x), addressof(y), sizeof(T)) == 0. I.e. if x == y,
// 	then x and y have the same bit pattern representation.

template <class T>
struct is_uniquely_represented
    : public std::integral_constant<bool,  std::is_integral_v<T> ||
                                           std::is_enum_v<T>     ||
                                           std::is_pointer_v<T>>
{};

template <class T>
inline constexpr bool is_uniquely_represented_v = is_uniquely_represented<T>::value;

template <class T>
struct is_uniquely_represented<T const>
    : public is_uniquely_represented<T>
{};

template <class T>
struct is_uniquely_represented<T volatile>
    : public is_uniquely_represented<T>
{};

template <class T>
struct is_uniquely_represented<T const volatile>
    : public is_uniquely_represented<T>
{};

// is_uniquely_represented<std::pair<T, U>>

template <class T, class U>
struct is_uniquely_represented<std::pair<T, U>>
    : public std::integral_constant<bool, is_uniquely_represented_v<T> &&
                                          is_uniquely_represented_v<U> &&
                                          sizeof(T) + sizeof(U) == sizeof(std::pair<T, U>)>
{
};

// is_uniquely_represented<std::tuple<T...>>

template <class ...T>
struct is_uniquely_represented<std::tuple<T...>>
    : public std::integral_constant<bool,
            (is_uniquely_represented_v<T> && ...) &&
            (sizeof(T) + ...) == sizeof(std::tuple<T...>)>
{
};

// is_uniquely_represented<T[N]>

template <class T, std::size_t N>
struct is_uniquely_represented<T[N]>
    : public is_uniquely_represented<T>
{
};

// is_uniquely_represented<std::array<T, N>>

template <class T, std::size_t N>
struct is_uniquely_represented<std::array<T, N>>
    : public std::integral_constant<bool, is_uniquely_represented_v<T> &&
                                          sizeof(T)*N == sizeof(std::array<T, N>)>
{
};

template <class T, class HashAlgorithm>
struct is_contiguously_hashable
    : public std::integral_constant<bool, is_uniquely_represented_v<T> &&
                                      (sizeof(T) == 1 ||
                                       byte_order_v<HashAlgorithm> == endian::native)>
{};

template <class T, std::size_t N, class HashAlgorithm>
struct is_contiguously_hashable<T[N], HashAlgorithm>
    : public std::integral_constant<bool, is_uniquely_represented_v<T[N]> &&
                                      (sizeof(T) == 1 ||
                                       byte_order_v<HashAlgorithm> == endian::native)>
{};

template<class T, class Hasher>
inline constexpr auto is_contiguously_hashable_v = is_contiguously_hashable<T, Hasher>::value;

}  // xstd

#endif  // HASH_TRAITS
