#include "xx_hash.h"
#include "Handle.h"
#include "hash_adaptors.h"
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
    using xstd::hash_append;
    hash_append(h, s.c, s.x, s.h);
}

template <class HashAlgorithm>
xstd::stream<HashAlgorithm>&
operator<<(xstd::stream<HashAlgorithm>& h, const S& s)
{
    return h << s.c << s.x << s.h;
}

int
main()
{
    xstd::shash<acme::xx_hash> my_hash;
    S s[10] = {'a', 1};
    std::cout << my_hash(s) << '\n';
}
