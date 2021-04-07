//--------------------------------- tuple.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_TUPLE_H
#define HASH_TUPLE_H

#include <tuple>
#include "hash/stream.h"

namespace hash
{

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

} // hash

#endif  // HASH_TUPLE_H
