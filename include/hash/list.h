//--------------------------------- list.h -------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_LIST_H
#define HASH_LIST_H

#include <list>
#include "hash/stream.h"

namespace hash
{

// list

template <class Hasher, class T, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::list<T, Alloc> const& l) noexcept
{
    return h << each(l) << l.size();
}

} // hash

#endif  // HASH_LIST_H
