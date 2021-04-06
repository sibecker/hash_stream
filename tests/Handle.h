//------------------------------- Handle.h -------------------------------------
// 
// This software is in the public domain.  The only restriction on its use is
// that no one can remove it from the public domain by claiming ownership of it,
// including the original authors.
// 
// There is no warranty of correctness on the software contained herein.  Use
// at your own risk.
// 
//------------------------------------------------------------------------------

#ifndef HANDLE_H
#define HANDLE_H

#include "hash/adaptors.h"

// namespace acme is used to demonstrate example code.  It is not proposed.

namespace acme
{

class Handle
{
    class CheshireCat;               // Not defined here
    CheshireCat* smile;               // Handle

public:
    Handle();                         // Default Constructor
    ~Handle();                        // Destructor
    Handle(Handle const&) = delete;
    Handle& operator=(Handle const&) = delete;
    Handle(int data1, int data2);
    // Other operations...

    // Hash support
    using type_erased_hasher = hash::type_erased_hasher<std::size_t>;
    using type_erased_stream = hash::type_erased_stream;

    friend
    void
    hash_append(type_erased_hasher&, Handle const&);

    template <class Hasher>
    friend
    void
    hash_append(Hasher& h, Handle const& x)
    {
        using hash::hash_append;
        if (x.smile == nullptr)
            hash_append(h, nullptr);
        else
        {
            type_erased_hasher temp(std::move(h));
            hash_append(temp, x);
            h = std::move(*temp.target<Hasher>());
        }
    }

    friend
    type_erased_stream&
    operator<<(type_erased_stream&&, Handle const&) noexcept;

    template<typename Hasher>
    friend
    hash::stream<Hasher>&
    operator<<(hash::stream<Hasher>& h, Handle const& x) noexcept
    {
        if (x.smile == nullptr)
            return h << nullptr;
        else {
            return type_erased_stream{h} << x;
        }
    }
};

}  // acme

#endif  // HANDLE_H
