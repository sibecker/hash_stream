#include "hash/append.h"
#include "hash/append.h"
#include <cstddef>

class MyHashAlgorithm
{
public:
    static constexpr xstd::endian endian = xstd::endian::native;

    using result_type = std::size_t;

    void
    operator()(void const* s, std::size_t length) noexcept
    {
        if (length >= 4 && length <= 8)
            return hash_4to8_bytes(s, length);
        if (length > 8 && length <= 16)
            return hash_9to16_bytes(s, length);
        if (length > 16 && length <= 32)
            return hash_17to32_bytes(s, length);
        if (length > 32)
            return hash_33to64_bytes(s, length);
        if (length != 0)
            return hash_1to3_bytes(s, length);
    }

    template<typename Hasher>
    void
    operator<<(std::span<std::byte const> const& bytes)
    {
        (*this)(bytes.data(), bytes.size());
    }

    std::size_t
    operator()() noexcept;

    explicit
    operator std::size_t() noexcept
    {
        return (*this)();
    }

private:
    void hash_1to3_bytes(void const* s, std::size_t length) noexcept;
    void hash_4to8_bytes(void const* s, std::size_t length) noexcept;
    void hash_9to16_bytes(void const* s, std::size_t length) noexcept;
    void hash_17to32_bytes(void const* s, std::size_t length) noexcept;
    void hash_33to64_bytes(void const* s, std::size_t length) noexcept;
};

std::size_t
test(int i)
{
    return xstd::shash<MyHashAlgorithm>{}(i);
}
