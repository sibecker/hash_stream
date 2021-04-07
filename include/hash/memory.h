//-------------------------------- memory.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_MEMORY_H
#define HASH_MEMORY_H

#include <memory>
#include "hash/stream.h"

namespace hash
{

// unique_ptr

template <class Hasher, class T, class Deleter>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unique_ptr<T, Deleter> const& p) noexcept
{
    return h << p.get();
}

// shared_ptr

template <class Hasher, class T>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::shared_ptr<T> const& p) noexcept
{
    return h << p.get();
}

// weak_ptr

template <class Hasher, class T>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::weak_ptr<T> const& p) noexcept
{
    return h << p.lock();
}

} // hash

#endif  // HASH_MEMORY_H
