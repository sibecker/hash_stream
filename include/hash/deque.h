//--------------------------------- deque.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_DEQUE_H
#define HASH_DEQUE_H

#include <deque>
#include "hash/stream.h"

namespace hash
{

// deque

template <class Hasher, class T, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::deque<T, Alloc> const& d) noexcept
{
    return h << each(d) << d.size();
}

} // hash

#endif  // HASH_DEQUE_H
