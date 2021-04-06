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
#include "hash/append.h"
#include "hash/stream.h"

// Example Hashers
#include "hash/spooky.h"
#include "hash/fnv1a.h"
#include "hash/jenkins1.h"
#include "hash/murmur2A.h"
#include "hash/spooky.h"
#include "hash/siphash.h"
#include "hash/functors.h"
#include "hash/adaptors.h"
#include "X.h"
#include "hash_test.h"
#include "hash/city_hash.h"
#include "hash/xx_hash.h"

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

    TEST(hash::uhash<hash::city>);
    TEST(hash::shash<hash::city>);

    TEST(hash::uhash<hash::fnv1a>);
    TEST(hash::shash<hash::fnv1a>);

    TEST(hash::uhash<hash::jenkins1>);
    TEST(hash::shash<hash::jenkins1>);

    TEST(hash::uhash<hash::MurmurHash2A>);
    TEST(hash::shash<hash::MurmurHash2A>);

    TEST(hash::uhash<hash::spooky>);
    TEST(hash::shash<hash::spooky>);

    TEST(hash::uhash<hash::siphash>);
    TEST(hash::shash<hash::siphash>);

    TEST(hash::uhash<hash::xx_hash>);
    TEST(hash::shash<hash::xx_hash>);
#undef TEST
}
