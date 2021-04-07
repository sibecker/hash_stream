//--------------------------------- array.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_ARRAY_H
#define HASH_ARRAY_H

#include <array>
#include "hash/stream.h"

namespace hash
{

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

} // hash

#endif  // HASH_ARRAY_H
