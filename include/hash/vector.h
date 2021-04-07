//-------------------------------- vector.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_VECTOR_H
#define HASH_VECTOR_H

#include <vector>
#include "hash/stream.h"

namespace hash
{

// vector

template <class Hasher, class T, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::vector<T, Alloc> const& v) noexcept
{
    return h << std::span<T const>{v.data(), v.size()} << v.size();
}

} // hash

#endif  // HASH_VECTOR_H
