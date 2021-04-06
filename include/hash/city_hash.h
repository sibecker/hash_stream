//------------------------------ city_hash.h -----------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_CITY_HASH_H
#define HASH_CITY_HASH_H

#include <cstddef>
#include <span>
#include <vector>
#include "hash/endian.h"

// namespace acme is used to demonstrate example code.  It is not proposed.

namespace hash
{

class city
{
    std::vector<char> buf_;
public:
    using result_type = std::size_t;

    void
    operator()(void const* key, std::size_t len) noexcept;

    void operator<<(std::span<std::byte const> const& bytes);

    explicit
    operator std::size_t() noexcept;

    std::size_t operator()() noexcept;
};

}  // hash

#endif  // HASH_CITY_HASH_H
