//------------------------------- test.cpp -------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

// The proposal
#include "hash_append.h"
#include "hash_stream.h"

// Example Hashers
#include "spooky.h"
#include "fnv1a.h"
#include "jenkins1.h"
#include "murmur2A.h"
#include "spooky.h"
#include "siphash.h"
#include "hash_functors.h"
#include "hash_adaptors.h"
#include "X.h"
#include "hash_test.h"
#include "city_hash.h"
#include "xx_hash.h"

#include <iostream>
#include <chrono>
#include <set>

int
main()
{
    typedef std::chrono::duration<float> secs;
    std::set<mine::X> x;
    while (x.size() < 1000000)
        x.insert(mine::X{});

    auto test = [&x](auto const& h, const char* name)
    {
        std::vector<std::size_t> hashes;
        hashes.reserve(x.size());
        {
            auto t0 = std::chrono::high_resolution_clock::now();
            for (auto const &i : x)
                hashes.push_back(h(i));
            auto t1 = std::chrono::high_resolution_clock::now();
            std::cout << name << " " << '\n' << std::hex;
            std::cout << secs(t1 - t0).count() << " s\n";
            std::cout << test1(hashes) << '\n';
            std::cout << test2(hashes) << '\n';
            std::cout << test3(hashes) << '\n';
            std::cout << test4(hashes) << '\n';
            std::cout << test5(hashes) << '\n';
        }
        std::cout << std::endl;
        return hashes;
    };
#define TEST(NAME) test(NAME{}, #NAME)

    TEST(std::hash<mine::X>);
#ifdef __clang__
    test(&hash_value, "llvm::hash_value");
#endif

    TEST(xstd::uhash<acme::city>);
    TEST(xstd::shash<acme::city>);

    TEST(xstd::uhash<acme::fnv1a>);
    TEST(xstd::shash<acme::fnv1a>);

    TEST(xstd::uhash<acme::jenkins1>);
    TEST(xstd::shash<acme::jenkins1>);

    TEST(xstd::uhash<acme::MurmurHash2A>);
    TEST(xstd::shash<acme::MurmurHash2A>);

    TEST(xstd::uhash<acme::spooky>);
    TEST(xstd::shash<acme::spooky>);

    TEST(xstd::uhash<acme::siphash>);
    TEST(xstd::shash<acme::siphash>);

    TEST(xstd::uhash<acme::xx_hash>);
    TEST(xstd::shash<acme::xx_hash>);
#undef TEST
}
