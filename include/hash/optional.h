//------------------------------- optional.h -----------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_OPTIONAL_H
#define HASH_OPTIONAL_H

#include <optional>
#include "hash/stream.h"

namespace hash
{

// nullopt_t

template <class Hasher>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::nullopt_t const&) noexcept
{
    return h << false;
}

// optional

template <class Hasher, class T, class Deleter>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::optional<T> const& o) noexcept
{
    if (o) {
        return h << true << *o;
    } else {
        return h << false;
    }
}

} // hash

#endif  // HASH_OPTIONAL_H
