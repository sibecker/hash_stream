//------------------------------- sha256.h -------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_SHA256_H
#define HASH_SHA256_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include "hash/endian.h"
#include "hash/details/sha2.h"

namespace hash
{

class sha256
{
    SHA256_CTX state_;
public: 
    static constexpr xstd::endian endian = xstd::endian::big;
    using result_type = std::array<std::uint8_t, SHA256_DIGEST_LENGTH>;

    sha256() noexcept
    {
        SHA256_Init(&state_);
    }

    void
    operator()(void const* key, std::size_t len) noexcept
    {
        SHA256_Update(&state_, static_cast<uint8_t const*>(key), len);
    }

    explicit
    operator result_type() noexcept
    {
        result_type r;
        SHA256_Final(r.data(), &state_);
        return r;
    }
};

}  // hash

#endif  // HASH_SHA256_H
