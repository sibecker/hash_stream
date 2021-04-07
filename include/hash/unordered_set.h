//----------------------------- unordered_set.h --------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_UNORDERED_SET_H
#define HASH_UNORDERED_SET_H

#include <unordered_set>
#include "hash/set.h"

namespace hash
{

// unordered_set

template <class Hasher, class Key, class Hash, class Pred, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unordered_set<Key, Hash, Pred, Alloc> const& s)
{
    using value_type = typename std::unordered_set<Key, Hash, Pred, Alloc>::value_type;
    return h << std::set<std::reference_wrapper<value_type const>, std::less<value_type>>(s.cbegin(), s.cend());
}

// unordered_multiset

template <class Hasher, class Key, class Hash, class Pred, class Alloc>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::unordered_multiset<Key, Hash, Pred, Alloc> const& s)
{
    using value_type = typename std::unordered_multiset<Key, Hash, Pred, Alloc>::value_type;
    return h << std::multiset<std::reference_wrapper<value_type const>, std::less<value_type>>(s.cbegin(), s.cend());
}

} // hash

#endif  // HASH_UNORDERED_SET_H
