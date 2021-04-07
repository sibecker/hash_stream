//-------------------------------- string.h ------------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_STRING_H
#define HASH_STRING_H

#include <string>
#include <string_view>
#include "hash/stream.h"

namespace hash
{

// basic_string_view

template <class Hasher, class CharT, class Traits>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::basic_string_view<CharT, Traits> const& s) noexcept
{
    return h << std::span<CharT const>{s.data(), s.size()} << s.size();
}

// basic_string

template <class Hasher, class CharT, class Traits, class Alloc>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::basic_string<CharT, Traits, Alloc> const& s) noexcept
{
    return h << std::basic_string_view<CharT, Traits>{s};
}

} // hash

#endif  // HASH_STRING_H
