//---------------------------------- set.h -------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_SET_H
#define HASH_SET_H

#include <set>
#include "hash/stream.h"

namespace hash
{

// set

template <class Hasher, class T, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::set<T, Comp, Alloc> const& s)
{
    return h << each(s) << s.size();
}

// multiset

template <class Hasher, class T, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::multiset<T, Comp, Alloc> const& s)
{
    return h << each(s) << s.size();
}

} // hash

#endif  // HASH_SET_H
