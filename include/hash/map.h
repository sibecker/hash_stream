//---------------------------------- map.h -------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <map>
#include "hash/stream.h"

namespace hash
{

// map

template <class Hasher, class Key, class T, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::map<Key, T, Comp, Alloc> const& m)
{
    return h << each(m) << m.size();
}

// multimap

template <class Hasher, class Key, class T, class Comp, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::multimap<Key, T, Comp, Alloc> const& m)
{
    return h << each(m) << m.size();
}

} // hash

#endif  // HASH_MAP_H
