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

#include "Handle.h"
#include "hash/set.h"

// namespace acme is used to demonstrate example code.  It is not proposed.

namespace acme
{

class Handle::CheshireCat
{
    std::set<int> data_;
public:
    CheshireCat(int data1, int data2)
    {
        data_.insert(data1);
        data_.insert(data2);
    }

    friend
    void
    hash_append(Handle::type_erased_hasher& h, Handle const& c);

    friend
    Handle::type_erased_stream&
    operator<<(Handle::type_erased_stream&& h, Handle const& c) noexcept;
};

void
hash_append(Handle::type_erased_hasher& h, Handle const& x)
{
    using hash::hash_append;
    {
        using hash::hash_append;
        if (x.smile == nullptr)
            hash_append(h, nullptr);
        else
        {
            auto const& c = *x.smile;
            hash_append(h, c.data_);
        }
    }
}

Handle::type_erased_stream&
operator<<(Handle::type_erased_stream&& h, Handle const& x) noexcept
{
    if (x.smile == nullptr)
        return h << nullptr;
    else
    {
        auto const& c = *x.smile;
        return h << c.data_;
    }
}

Handle::Handle()
    : smile(new CheshireCat(1, 2))
{
}

Handle::Handle(int data1, int data2)
    : smile(new CheshireCat(data1, data2))
{
}

Handle::~Handle()
{
    delete smile;
}

}  // acme
