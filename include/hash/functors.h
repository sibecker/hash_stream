//---------------------------- hash_functors.h ---------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HASH_FUNCTORS_H
#define HASH_FUNCTORS_H

#include <cstddef>
#include <random>
#include <mutex>
#include <tuple>
#include "hash/siphash.h"
#include "hash/stream.h"

namespace hash
{

// A hasher that can be seeded with up to two seeds
template <class Hasher = siphash>
class seeded_hash
{
private:
    std::size_t seed0_;
    std::size_t seed1_;
public:
    using result_type = typename stream<Hasher>::result_type;

    explicit seeded_hash(std::size_t seed0 = 0, std::size_t seed1 = 0)
        : seed0_(seed0)
        , seed1_(seed1)
    {}

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        stream<Hasher> h(seed0_, seed1_);
        h << t;
        return h();
    }
};

// A hasher that can be seeded, or defaults to a random seed
template <class Hasher = siphash>
class randomly_seeded_hash
{
private:
    static std::mutex mut_s;
    static std::mt19937_64 rand_s;

    std::size_t seed0_;
    std::size_t seed1_;
public:
    using result_type = typename stream<Hasher>::result_type;

    randomly_seeded_hash()
    {
        std::lock_guard<std::mutex> _(mut_s);
        seed0_ = rand_s();
        seed1_ = rand_s();
    }

    explicit randomly_seeded_hash(std::size_t seed0, std::size_t seed1 = 0)
        : seed0_(seed0)
        , seed1_(seed1)
    {}

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        stream<Hasher> h(seed0_, seed1_);
        h << t;
        return h();
    }
};

template <class Hasher>
std::mutex
randomly_seeded_hash<Hasher>::mut_s;

template <class Hasher>
std::mt19937_64
randomly_seeded_hash<Hasher>::rand_s{std::random_device{}()};

// A hasher that can be seeded, or defaults to a random seed set per process

std::tuple<std::uint64_t, std::uint64_t>
get_process_seed();

template <class Hasher = siphash>
class process_seeded_hash
{
public:
    using result_type = typename stream<Hasher>::result_type;

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        std::uint64_t seed0;
        std::uint64_t seed1;
        std::tie(seed0, seed1) = get_process_seed();
        Hasher h(seed0, seed1);
        h << t;
        return h();
    }
};

// A hasher that prepends salt
template <class Hasher = siphash>
class salted_hash
{
private:
    std::size_t salt_;
public:
    using result_type = typename stream<Hasher>::result_type;

    explicit salted_hash(std::size_t salt)
        : salt_(salt)
    {}

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        stream<Hasher> h;
        h << salt_ << t;
        return h();
    }
};

// A hasher that adds padding
template <class Hasher = siphash>
class padded_hash
{
private:
    std::string prefix_;
    std::string postfix_;
public:
    using result_type = typename stream<Hasher>::result_type;

    padded_hash(std::string const& prefix, std::string const& postfix)
        : prefix_(prefix)
        , postfix_(postfix)
    {}

    template <class T>
    result_type
    operator()(T const& t) const noexcept
    {
        stream<Hasher> h;
        h << prefix_ + " GG " << t << " RR " + postfix_;
        return h();
    }
};

}  // hash

#endif  // HASH_FUNCTORS_H
