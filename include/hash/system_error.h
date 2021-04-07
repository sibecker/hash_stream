//----------------------------- system_error.h ---------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#ifndef HASH_SYSTEM_ERROR_H
#define HASH_SYSTEM_ERROR_H

#include <system_error>
#include "hash/stream.h"

namespace hash
{

// error_code

template <class HashAlgorithm>
inline
stream<HashAlgorithm>&
operator<<(stream<HashAlgorithm>& h, std::error_code const& ec)
{
    return h << ec.value() << &ec.category();
}

} // hash

#endif  // HASH_SYSTEM_ERROR_H
