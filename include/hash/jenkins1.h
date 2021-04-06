//------------------------------- jenkins1.h -----------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_JENKINS1_H
#define HASH_JENKINS1_H

#include <cstddef>
#include <span>
#include "hash/endian.h"

namespace hash
{

class jenkins1
{
    std::size_t state_ = 0;

public:
    static constexpr endian endian = endian::native;
    using result_type = std::size_t;

    void
    operator()(void const* key, std::size_t len) noexcept
    {
        unsigned char const* p = static_cast<unsigned char const*>(key);
        unsigned char const* const e = p + len;
        for (; p < e; ++p)
        {
            state_ += *p;
            state_ += state_ << 10;
            state_ ^= state_ >> 6;
        }
    }

    void
    operator<<(std::span<std::byte const> const& bytes) noexcept
    {
        return (*this)(bytes.data(), bytes.size());
    }

    std::size_t
    operator()() noexcept
    {
        state_ += state_ << 3;
        state_ ^= state_ >> 11;
        state_ += state_ << 15;
        return state_;
    }

    explicit
    operator std::size_t() noexcept
    {
        return (*this)();
    }
};

}  // hash

#endif  // HASH_JENKINS1_H
