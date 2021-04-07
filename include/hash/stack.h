//--------------------------------- stack.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_STACK_H
#define HASH_STACK_H

#include <stack>
#include "hash/stream.h"

namespace hash
{

// stack

template <class Hasher, class T, class Container>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::stack<T, Container> const& s) noexcept
{
    return h << each(s) << s.size();
}

} // hash

#endif  // HASH_STACK_H
