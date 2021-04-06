//-------------------------------- stream.h ------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_STREAM
#define HASH_STREAM

#include "hash/traits.h"
#include "hash/siphash.h"  // the current default hasher
#include <map>
#include <memory>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <system_error>

// Everything in namespace xstd, excluding those items in xstd::detail,
//  is proposed.

// C++14 is assumed below because std::index_sequence_for makes hash_append
// 	for tuple just so easy.  So in for a penny, in for a pound...

namespace hash
{

// template <class Hasher, class T>
// stream<Hasher>&
// operator<<(stream<Hasher>& h, T const& t);
// 
// Each type to be hashed must either be contiguously hashable, or overload
// this operator to expose its hashable bits to a Hasher.

template<typename Hasher>
class stream
{
public:
    template<typename... Args>
    stream(Args&& ... args) : hasher_{std::forward<Args>(args)...}
    {}

    using result_type = typename std::decay_t<decltype(std::declval<Hasher>()())>;

    stream&
    operator<<(std::span<std::byte const> const& rhs) noexcept
    {
        hasher_ << rhs;
        return *this;
    }

    result_type operator()() noexcept
    {
        return hasher_();
    }

private:
    Hasher hasher_;
};

// span

template<typename Hasher, typename T>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::span<T const> const& rhs) noexcept
{
    if constexpr (is_contiguously_hashable_v<T, Hasher>) {
        return h << std::as_bytes(rhs);
    } else {
        for(auto const& value : rhs) {
            h << value;
        }
        return h;
    }
}

// scalars

template <class Hasher, class T>
inline
std::enable_if_t<
    is_contiguously_hashable_v<T, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T const& t) noexcept
{
    std::span<std::byte const> bytes(reinterpret_cast<std::byte const*>(std::addressof(t)), sizeof(t));
    return h << bytes;
}

template <class Hasher, class T>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher> &&
    (std::is_integral_v<T> || std::is_pointer_v<T> || std::is_enum_v<T>),
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T t) noexcept
{
    detail::reverse_bytes(t);
    return h << std::span<std::byte const>{std::addressof(t), sizeof(t)};
}

template <class Hasher, class T>
inline
std::enable_if_t
<
    std::is_floating_point_v<T>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T t) noexcept
{
    if (t == 0)
        t = 0;
    detail::maybe_reverse_bytes(t, h);
    return h << std::span<std::byte const>{&t, sizeof(t)};
}

template <class Hasher>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::nullptr_t) noexcept
{
    void const* const p = nullptr;
    detail::maybe_reverse_bytes(p, h);
    return h << std::span<std::byte const>{reinterpret_cast<std::byte const*>(&p), sizeof(p)};
}

// Forward declarations for ADL purposes

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T (&a)[N]) noexcept;

template <class Hasher, class CharT, class Traits>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::basic_string_view<CharT, Traits> const& s) noexcept;

template <class Hasher, class CharT, class Traits, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept;

template <class Hasher, class T, class U>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::pair<T, U>, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, std::pair<T, U> const& p) noexcept;

template <class Hasher, class T, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::vector<T, Alloc> const& v) noexcept;

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::array<T, N>, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, std::array<T, N> const& a) noexcept;

template <class Hasher, class ...T>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::tuple<T...>, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, std::tuple<T...> const& t) noexcept;

template <class Hasher, class Key, class T, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::map<Key, T, Comp, Alloc> const& m);

template <class Hasher, class Key, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::set<Key, Comp, Alloc> const& s);

template <class Hasher, class Key, class T, class Hash, class Pred, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unordered_map<Key, T, Hash, Pred, Alloc> const& m);

template <class Hasher, class Key, class Hash, class Pred, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unordered_set<Key, Hash, Pred, Alloc> const& s);

// c-array

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T (&a)[N]) noexcept
{
    return h << std::span<T const>{a};
}

// basic_string_view

template <class Hasher, class CharT, class Traits>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::basic_string_view<CharT, Traits> const& s) noexcept
{
    return h << std::span<CharT const>{s.data(), s.size()} << s.size();
}

// basic_string

template <class Hasher, class CharT, class Traits, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept
{
    return h << std::basic_string_view<CharT, Traits>{s};
}

// pair

template <class Hasher, class T, class U>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<std::pair<T, U>, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, std::pair<T, U> const& p) noexcept
{
    return h << p.first << p.second;
}

// vector

template <class Hasher, class T, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::vector<T, Alloc> const& v) noexcept
{
    return h << std::span<T const>{v.data(), v.size()} << v.size();
}

// array

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::array<T, N>, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, std::array<T, N> const& a) noexcept
{
    return h << std::span<T const>(a);
}

// tuple

template <class Hasher, class ...T>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<std::tuple<T...>, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, std::tuple<T...> const& t) noexcept
{
    auto send = [&h](T const&... args) -> stream<Hasher>&
    {
        return (h << ... << args);
    };
    return std::apply(send, t);
}

// map

template <class Hasher, class Key, class T, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::map<Key, T, Comp, Alloc> const& m)
{
    for(auto const& entry : m)
        h << entry;
    return h;
}

// set

template <class Hasher, class Key, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::set<Key, Comp, Alloc> const& s)
{
    for(auto const& entry : s)
        h << entry;
    return h;
}

// error_code

template <class HashAlgorithm>
inline
stream<HashAlgorithm>&
operator<<(stream<HashAlgorithm>& h, std::error_code const& ec)
{
    return h << ec.value() << &ec.category();
}

// shash

template <class Hasher = siphash>
struct shash
{
    using result_type = typename stream<Hasher>::result_type;

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        stream<Hasher> h;
        h << t;
        return h();
    }
};

} // hash

#endif  // HASH_STREAM
