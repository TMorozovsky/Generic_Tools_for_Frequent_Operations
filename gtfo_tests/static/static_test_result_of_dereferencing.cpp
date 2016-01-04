#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#define GTFO_RESULT_OF_DEREFERENCING(t) typename ::gtfo::_tt::result_of_dereferencing<t>::type
#define GTFO_RESULT_OF_DEREFERENCING_IS(t,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_DEREFERENCING(t), r>::value

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct X
    {
        A operator * ()                { return A(); }
        B operator * () const          { return B(); }
        C operator * () volatile       { return C(); }
        D operator * () const volatile { return D(); }
    };
}

static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(int *, int &), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(const int *, const int &), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(volatile int *, volatile int &), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(const volatile int *, const volatile int &), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(X, A), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(X &, A), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(const X, B), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(volatile X, C), "");
static_assert(GTFO_RESULT_OF_DEREFERENCING_IS(const volatile X, D), "");
