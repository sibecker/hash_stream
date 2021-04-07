//--------------------------------- queue.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_QUEUE_H
#define HASH_QUEUE_H

#include <queue>
#include "hash/stream.h"

namespace hash
{

// queue

template <class Hasher, class T, class Container>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::queue<T, Container> const& q) noexcept
{
    return h << each(q) << q.size();
}

} // hash

#endif  // HASH_QUEUE_H
