//----------------------------- unordered_map.h --------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_UNORDERED_MAP_H
#define HASH_UNORDERED_MAP_H

#include <unordered_map>
#include "hash/tuple.h"
#include "hash/set.h"

namespace hash
{

// unordered_map

template <class Hasher, class Key, class T, class Hash, class Pred, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unordered_map<Key, T, Hash, Pred, Alloc> const& m)
{
    using value_type = typename std::unordered_map<Key, T, Hash, Pred, Alloc>::value_type;
    return h << std::set<std::reference_wrapper<value_type const>, std::less<value_type>>(m.cbegin(), m.cend());
}

// unordered_multimap

template <class Hasher, class Key, class T, class Hash, class Pred, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unordered_multimap<Key, T, Hash, Pred, Alloc> const& m)
{
    using value_type = typename std::unordered_multimap<Key, T, Hash, Pred, Alloc>::value_type;
    return h << std::multiset<std::reference_wrapper<value_type const>, std::less<value_type>>(m.cbegin(), m.cend());
}

} // hash

#endif  // HASH_UNORDERED_MAP_H
