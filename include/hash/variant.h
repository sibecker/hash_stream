//-------------------------------- variant.h -----------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_VARIANT_H
#define HASH_VARIANT_H

#include <variant>
#include "hash/stream.h"

namespace hash
{

// monostate

template <class Hasher>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::monostate const&) noexcept
{
    return h;
}

// variant

template <class Hasher, class... T>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::variant<T...> const& v) noexcept
{
    auto const send = [&h, &v](auto const& value) -> stream<Hasher>&
    {
        return h << v.which() << value;
    };
    return std::visit(send, v);
}

} // hash

#endif  // HASH_VARIANT_H
