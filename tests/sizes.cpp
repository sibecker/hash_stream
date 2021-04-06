//------------------------------- sizes.cpp ------------------------------------
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
#include "hash/stream.h"

// Example Hashers
#include "hash/siphash.h"
#include "hash/fnv1a.h"
#include "hash/spooky.h"

#include <iostream>
int
main()
{
    std::cout << sizeof(hash::siphash) << '\n';
    std::cout << sizeof(hash::spooky) << '\n';
    std::cout << sizeof(hash::fnv1a) << '\n';
}

// 48
// 304
// 8
