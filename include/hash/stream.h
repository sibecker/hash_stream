//-------------------------------- stream.h ------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_STREAM_H
#define HASH_STREAM_H

#include <span>
#include "hash/siphash.h"  // the current default hasher
#include "hash/traits.h"

// Everything in namespace xstd, excluding those items in xstd::detail,
//  is proposed.

// C++20 is assumed below because std::span<std::byte const> is such
// a good fit for streaming bytes.  So in for a penny, in for a pound...

namespace hash
{

// template <class Hasher, class T>
// stream<Hasher>&
// operator<<(stream<Hasher>& h, T const& t);
// 
// Each type to be hashed must either be contiguously hashable, or overload
// this operator to expose its hashable bits to a Hasher.

template<typename Hasher>
class stream
{
public:
    template<typename... Args>
    stream(Args&& ... args) : hasher_{std::forward<Args>(args)...}
    {}

    using result_type = typename std::decay_t<decltype(std::declval<Hasher>()())>;

    stream&
    operator<<(std::span<std::byte const> const& rhs) noexcept
    {
        hasher_ << rhs;
        return *this;
    }

    result_type operator()() noexcept
    {
        return hasher_();
    }

private:
    Hasher hasher_;
};

// Very handy utility class

template<class Container>
class each
{
private:
    Container const& container_;

public:
    explicit
    each(Container const& container)
        : container_{container}
    {}

    template<typename Hasher>
    friend
    stream<Hasher>&
    operator<<(stream<Hasher>& h, each const& e)
    {
        for(auto const& value : e.container_)
            h << value;
        return h;
    }
};

// span
// span lacks an equality operator so we ar free to use it to mean whater we want it to mean
// In this case, we treat equality the same way a vector or array would

template<typename Hasher, typename T>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::span<T const> const& s) noexcept
{
    if constexpr (is_contiguously_hashable_v<T, Hasher>) {
        return h << std::as_bytes(s);
    } else {
        return h << each(s);
    }
}

// reference_wrapper

template<typename Hasher, typename T>
stream<Hasher>&
operator<<(stream<Hasher>& h, std::reference_wrapper<T const> const& r) noexcept
{
    return h<< r.get();
}

// scalars

template <class Hasher, class T>
inline
std::enable_if_t<
    is_contiguously_hashable_v<T, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T const& t) noexcept
{
    std::span<std::byte const> bytes(reinterpret_cast<std::byte const*>(std::addressof(t)), sizeof(t));
    return h << bytes;
}

template <class Hasher, class T>
inline
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher> &&
    (std::is_integral_v<T> || std::is_pointer_v<T> || std::is_enum_v<T>),
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T t) noexcept
{
    detail::reverse_bytes(t);
    std::span<std::byte const> bytes(reinterpret_cast<std::byte const*>(std::addressof(t)), sizeof(t));
    return h << bytes;
}

template <class Hasher, class T>
inline
std::enable_if_t
<
    std::is_floating_point_v<T>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T t) noexcept
{
    if (t == 0)
        t = 0;
    detail::maybe_reverse_bytes(t, h);
    return h << std::span<std::byte const>{&t, sizeof(t)};
}

template <class Hasher>
inline
stream<Hasher>&
operator<<(stream<Hasher>& h, std::nullptr_t) noexcept
{
    void const* const p = nullptr;
    detail::maybe_reverse_bytes(p, h);
    return h << p;
}

// c-array

template <class Hasher, class T, std::size_t N>
std::enable_if_t
<
    !is_contiguously_hashable_v<T, Hasher>,
    stream<Hasher>&
>
operator<<(stream<Hasher>& h, T (&a)[N]) noexcept
{
    return h << std::span<T const>{a};
}

// shash

template <class Hasher = siphash>
struct shash
{
    using result_type = typename stream<Hasher>::result_type;

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        stream<Hasher> h;
        h << t;
        return h();
    }
};

} // hash

#endif  // HASH_STREAM_H
