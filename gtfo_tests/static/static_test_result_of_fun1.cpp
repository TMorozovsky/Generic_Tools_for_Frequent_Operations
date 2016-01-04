#include "gtfo/_impl/type_traits/result_of_fun1.hpp"
#define GTFO_RESULT_OF_FUN1(f,a) typename ::gtfo::_tt::result_of_fun1<f,a>::type
#define GTFO_RESULT_OF_FUN1_IS(f,a,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_FUN1(f,a), r>::value

namespace
{

    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct E { };
    inline A foo(signed char) { return A(); }
    struct X
    {
        B operator () (signed char)                { return B(); }
        C operator () (signed char) const          { return C(); }
        D operator () (signed char) volatile       { return D(); }
        E operator () (signed char) const volatile { return E(); }
    };
}

static_assert(GTFO_RESULT_OF_FUN1_IS(decltype(foo), long, A), "");
static_assert(GTFO_RESULT_OF_FUN1_IS(X, long, B), "");
static_assert(GTFO_RESULT_OF_FUN1_IS(X &, long, B), "");
static_assert(GTFO_RESULT_OF_FUN1_IS(const X, long, C), "");
static_assert(GTFO_RESULT_OF_FUN1_IS(volatile X, long, D), "");
static_assert(GTFO_RESULT_OF_FUN1_IS(const volatile X, long, E), "");
