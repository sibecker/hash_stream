//----------------------------- forward_list.h ---------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_FORWARD_LIST_H
#define HASH_FORWARD_LIST_H

#include <forward_list>
#include "hash/stream.h"

namespace hash
{

// forward_list

template <class Hasher, class T, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::forward_list<T, Alloc> const& l) noexcept
{
    std::size_t size = 0;
    for(auto const& value : l) {
        h << l;
        ++size;
    }
    return h << size;
}

} // hash

#endif  // HASH_FORWARD_LIST_H
