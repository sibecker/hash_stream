#include "hash/xx_hash.h"
#include "Handle.h"
#include "hash/adaptors.h"
#include <iostream>

struct S
{
    char c;
    int x;
    acme::Handle h;
};

template <class HashAlgorithm>
void
hash_append(HashAlgorithm& h, const S& s)
{
    using hash::hash_append;
    hash_append(h, s.c, s.x, s.h);
}

template <class HashAlgorithm>
hash::stream<HashAlgorithm>&
operator<<(hash::stream<HashAlgorithm>& h, const S& s)
{
    return h << s.c << s.x << s.h;
}

int
main()
{
    hash::shash<hash::xx_hash> my_hash;
    S s[10] = {'a', 1};
    std::cout << my_hash(s) << '\n';
}
