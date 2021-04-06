//---------------------------- hash_adaptors.h ---------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_ADAPTORS_H
#define HASH_ADAPTORS_H

#include <cstddef>
#include <functional>
#include "hash/stream.h"
#include "hash/append.h"

// namespace acme is used to demonstrate example code.  It is not proposed.

namespace hash
{

// Adapt another hasher to append the total length of the message
//   (not including the length of the length) to the message
template <class Hasher>
class postfix_length
{
    std::size_t length_ = 0;
    Hasher hasher_;

public:
    using result_type = typename Hasher::result_type;

    postfix_length() = default;
    explicit postfix_length(Hasher const& hasher)
        : hasher_(hasher)
    {}

    void
    operator()(void const* key, std::size_t len) noexcept
    {
        length_ += len;
        hasher_(key, len);
    }

    void
    operator<<(std::span<std::byte const> const& bytes) noexcept
    {
        length_ += bytes.size();
        hasher_(bytes);
    }

    explicit
    operator result_type() noexcept
    {
        hash_append(hasher_, length_);
        return static_cast<result_type>(hasher_);
    }

    result_type
    operator()() noexcept
    {
        hasher_ << length_;
        return hasher_();
    }
};

template <class ResultType>
class type_erased_hasher
{
public:
    using result_type = ResultType;
    
private:    
    using function = std::function<void(void const*, std::size_t)>;

    function hasher_;
    result_type (*convert_)(function&);

public:
    template <class Hasher,
                 class = std::enable_if_t
                 <
                      std::is_constructible<function, Hasher>{} &&
                      std::is_same<typename std::decay_t<Hasher>::result_type,
                                   result_type>{}
                 >
             >
    explicit
    type_erased_hasher(Hasher&& h)
        : hasher_(std::forward<Hasher>(h))
        , convert_(convert<std::decay_t<Hasher>>)
    {
    }

    void
    operator()(void const* key, std::size_t len)
    {
        hasher_(key, len);
    }

    explicit
    operator result_type() noexcept
    {
        return convert_(hasher_);
    }

    template <class T>
    T*
    target() noexcept
    {
        return hasher_.target<T>();
    }

private:
    template <class Hasher>
    static
    result_type
    convert(function& f) noexcept
    {
        return static_cast<result_type>(*f.target<Hasher>());;
    }
};

namespace detail
{
    struct type_erased_tag;
}

// Specialised version of stream, to become a type erased hash stream
// We only need to provide the operator<< as operator() will never
// be called directly.

template<>
class stream<detail::type_erased_tag>
{
private:
    void* stream_;
    std::function<void(std::span<std::byte const> const&)> function_;

public:
    template<class Hasher>
    stream(stream<Hasher>& h)
        : stream_{&h},
          function_{[&h](std::span<std::byte const> const& bytes)
            {
                h << bytes;
            }
        }
    {}

    stream&
    operator<<(std::span<std::byte const> const& bytes) noexcept
    {
        function_(bytes);
        return *this;
    }

    template<typename Hasher>
    operator stream<Hasher>& () noexcept
    {
        return *static_cast<stream<Hasher>*>(stream_);
    }
};
using type_erased_stream = typename stream<detail::type_erased_tag>;

} // hash

#endif  // HASH_ADAPTORS_H
