//------------------------------- spooky.h -------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_SPOOKY_H
#define HASH_SPOOKY_H

#include <cstddef>
#include <cstdint>
#include <span>
#include "hash/endian.h"
#include "hash/detail/SpookyV2.h"

namespace hash
{

class spooky
{
    detail::SpookyHash state_;
public: 
    static constexpr endian endian = endian::native;
    using result_type = std::size_t;

    spooky(std::size_t seed1 = 1, std::size_t seed2 = 2) noexcept
    {
        state_.Init(seed1, seed2);
    }

    void
    operator()(void const* key, std::size_t len) noexcept
    {
        state_.Update(key, len);
    }

    void
    operator<<(std::span<std::byte const> const& bytes) noexcept
    {
        return (*this)(bytes.data(), bytes.size());
    }

    explicit
    operator result_type() noexcept
    {
        return (*this)();
    }

    result_type
    operator()() noexcept
    {
        std::uint64_t h1, h2;
        state_.Final(&h1, &h2);
        return h1;
    }
};

}  // hash

#endif  // SPOOKY_H
