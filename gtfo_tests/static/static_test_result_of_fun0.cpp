#include "gtfo/_impl/type_traits/result_of_fun0.hpp"
#define GTFO_RESULT_OF_FUN0(f) typename ::gtfo::_tt::result_of_fun0<f>::type
#define GTFO_RESULT_OF_FUN0_IS(f,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_FUN0(f), r>::value

namespace
{

    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct E { };
    inline A foo() { return A(); }
    struct X
    {
        B operator () ()                { return B(); }
        C operator () () const          { return C(); }
        D operator () () volatile       { return D(); }
        E operator () () const volatile { return E(); }
    };
}

static_assert(GTFO_RESULT_OF_FUN0_IS(decltype(foo), A), "");
static_assert(GTFO_RESULT_OF_FUN0_IS(X, B), "");
static_assert(GTFO_RESULT_OF_FUN0_IS(X &, B), "");
static_assert(GTFO_RESULT_OF_FUN0_IS(const X, C), "");
static_assert(GTFO_RESULT_OF_FUN0_IS(volatile X, D), "");
static_assert(GTFO_RESULT_OF_FUN0_IS(const volatile X, E), "");
