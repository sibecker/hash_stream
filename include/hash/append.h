//-------------------------------- append.h ------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_APPEND_H
#define HASH_APPEND_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <system_error>

#include "hash/traits.h"

// Everything in namespace xstd, excluding those items in xstd::detail,
//  is proposed.

// C++14 is assumed below because std::index_sequence_for makes hash_append
// 	for tuple just so easy.  So in for a penny, in for a pound...

namespace hash
{

// template <class Hasher, class T>
// void
// hash_append(Hasher& h, T const& t);
// 
// Each type to be hashed must either be contiguously hashable, or overload
// 	hash_append to expose its hashable bits to a Hasher.

// scalars

template <class Hasher, class T>
inline
std::enable_if_t<
    is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, T const& t) noexcept
{
    h(std::addressof(t), sizeof(t));
}

template <class Hasher, class T>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher> &&
    (std::is_integral_v<T> || std::is_pointer_v<T> || std::is_enum_v<T>)
>
hash_append(Hasher& h, T t) noexcept
{
    detail::reverse_bytes(t);
    h(std::addressof(t), sizeof(t));
}

template <class Hasher, class T>
inline
std::enable_if_t
<
    std::is_floating_point_v<T>
>
hash_append(Hasher& h, T t) noexcept
{
    if (t == 0)
        t = 0;
    detail::maybe_reverse_bytes(t, h);
    h(&t, sizeof(t));
}

template <class Hasher>
inline
void
hash_append(Hasher& h, std::nullptr_t) noexcept
{
    void const* p = nullptr;
    detail::maybe_reverse_bytes(p, h);
    h(&p, sizeof(p));
}

// Forward declarations for ADL purposes

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, T (&a)[N]) noexcept;

template <class Hasher, class CharT, class Traits, class Alloc>
std::enable_if_t
<
    !is_contiguously_hashable_v<CharT, Hasher>
>
hash_append(Hasher& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept;

template <class Hasher, class CharT, class Traits, class Alloc>
std::enable_if_t
<
    is_contiguously_hashable_v<CharT, Hasher>
>
hash_append(Hasher& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept;

template <class Hasher, class T, class U>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::pair<T, U>, Hasher>
>
hash_append (Hasher& h, std::pair<T, U> const& p) noexcept;

template <class Hasher, class T, class Alloc>
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, std::vector<T, Alloc> const& v) noexcept;

template <class Hasher, class T, class Alloc>
std::enable_if_t
<
    is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, std::vector<T, Alloc> const& v) noexcept;

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::array<T, N>, Hasher>
>
hash_append(Hasher& h, std::array<T, N> const& a) noexcept;

template <class Hasher, class ...T>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::tuple<T...>, Hasher>
>
hash_append(Hasher& h, std::tuple<T...> const& t) noexcept;

template <class Hasher, class Key, class T, class Comp, class Alloc>
void
hash_append(Hasher& h, std::map<Key, T, Comp, Alloc> const& m);

template <class Hasher, class Key, class Comp, class Alloc>
void
hash_append(Hasher& h, std::set<Key, Comp, Alloc> const& s);

template <class Hasher, class Key, class T, class Hash, class Pred, class Alloc>
void
hash_append(Hasher& h, std::unordered_map<Key, T, Hash, Pred, Alloc> const& m);

template <class Hasher, class Key, class Hash, class Pred, class Alloc>
void
hash_append(Hasher& h, std::unordered_set<Key, Hash, Pred, Alloc> const& s);

template <class Hasher, class T0, class T1, class ...T>
void
hash_append (Hasher& h, T0 const& t0, T1 const& t1, T const& ...t) noexcept;

// c-array

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, T (&a)[N]) noexcept
{
    for (auto const& t : a)
        hash_append(h, t);
}

// basic_string

template <class Hasher, class CharT, class Traits, class Alloc>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<CharT, Hasher>
>
hash_append(Hasher& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept
{
    for (auto c : s)
        hash_append(h, c);
    hash_append(h, s.size());
}

template <class Hasher, class CharT, class Traits, class Alloc>
inline
std::enable_if_t
<
    is_contiguously_hashable_v<CharT, Hasher>
>
hash_append(Hasher& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept
{
    h(s.data(), s.size()*sizeof(CharT));
    hash_append(h, s.size());
}

// pair

template <class Hasher, class T, class U>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<std::pair<T, U>, Hasher>
>
hash_append (Hasher& h, std::pair<T, U> const& p) noexcept
{
    hash_append (h, p.first, p.second);
}

// vector

template <class Hasher, class T, class Alloc>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, std::vector<T, Alloc> const& v) noexcept
{
    for (auto const& t : v)
        hash_append(h, t);
    hash_append(h, v.size());
}

template <class Hasher, class T, class Alloc>
inline
std::enable_if_t
<
    is_contiguously_hashable_v<T, Hasher>
>
hash_append(Hasher& h, std::vector<T, Alloc> const& v) noexcept
{
    h(v.data(), v.size()*sizeof(T));
    hash_append(h, v.size());
}

// array

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<std::array<T, N>, Hasher>
>
hash_append(Hasher& h, std::array<T, N> const& a) noexcept
{
    for (auto const& t : a)
        hash_append(h, t);
}

// tuple

template <class Hasher, class ...T>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<std::tuple<T...>, Hasher>
>
hash_append(Hasher& h, std::tuple<T...> const& t) noexcept
{
    auto append = [&h](T const&... args)
    {
        hash_append(h, args...);
    };
    std::apply(append, t);
}

// map

template <class Hasher, class Key, class T, class Comp, class Alloc>
void
hash_append(Hasher& h, std::map<Key, T, Comp, Alloc> const& m)
{
    for(auto const& entry : m)
        hash_append(h, entry);
}

template <class Hasher, class Key, class Comp, class Alloc>
void
hash_append(Hasher& h, std::set<Key, Comp, Alloc> const& s)
{
    for(auto const& entry : s)
        hash_append(h, entry);
}


// variadic

template <class Hasher, class T0, class T1, class ...T>
inline
void
hash_append (Hasher& h, T0 const& t0, T1 const& t1, T const& ...t) noexcept
{
    hash_append(h, t0);
    hash_append(h, t1, t...);
}

// error_code

template <class HashAlgorithm>
inline
void
hash_append(HashAlgorithm& h, std::error_code const& ec)
{
    hash_append(h, ec.value(), &ec.category());
}

// uhash

template <class Hasher = siphash>
struct uhash
{
    using result_type = typename Hasher::result_type;

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        Hasher h;
        hash_append(h, t);
        return static_cast<result_type>(h);
    }
};

}  // hash

#endif  // HASH_APPEND_H
