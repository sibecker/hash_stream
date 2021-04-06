//-------------------------------- fnv1a.h -------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_FNV1A_H
#define HASH_FNV1A_H

#include <cstddef>
#include <span>
#include "hash/endian.h"

namespace hash
{

class fnv1a
{
    std::size_t state_ = 14695981039346656037u;
public:

    static constexpr endian endian = endian::native;
    using result_type = std::size_t;

    void
    operator()(void const* key, std::size_t len) noexcept
    {
        unsigned char const* p = static_cast<unsigned char const*>(key);
        unsigned char const* const e = p + len;
        for (; p < e; ++p)
            state_ = (state_ ^ *p) * 1099511628211u;
    }

    void
    operator<<(std::span<std::byte const> const& bytes) noexcept
    {
        for (auto const& b : bytes)
            state_ = (state_ ^ static_cast<unsigned char>(b)) * 1099511628211u;
    }

    std::size_t
    operator()() noexcept
    {
        return state_;
    }

    explicit
    operator std::size_t() noexcept
    {
        return (*this)();
    }
};

}  // hash

#endif  // HASH_FNV1A_H
