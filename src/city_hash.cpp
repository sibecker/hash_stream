//----------------------------- city_hash.cpp ----------------------------------
//
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
//
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
//
//------------------------------------------------------------------------------

#include <cstddef>
#include <span>
#include <vector>
#include "city.h"
#include "hash/city_hash.h"

// namespace acme is used to demonstrate example code.  It is not proposed.

namespace hash
{

void
city::operator()(void const* key, std::size_t len) noexcept
{
    char const* p = static_cast<char const*>(key);
    char const* const e = p + len;
    for (; p < e; ++p)
        buf_.push_back(*p);
}

void
city::operator<<(std::span<std::byte const> const& bytes)
{
    (*this)(bytes.data(), bytes.size());
}

city::operator std::size_t() noexcept
{
    return (*this)();
}

std::size_t
city::operator()() noexcept
{
    return CityHash64(buf_.data(), buf_.size());
}

}  // hash

