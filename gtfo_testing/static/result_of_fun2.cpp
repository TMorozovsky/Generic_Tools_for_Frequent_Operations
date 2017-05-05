#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#define GTFO_RESULT_OF_FUN2(f,a,b) typename ::gtfo::_tt::result_of_fun2<f,a,b>::type
#define GTFO_RESULT_OF_FUN2_IS(f,a,b,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_FUN2(f,a,b), r>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct E { };
    inline UNUSED A foo(signed char, float) { return A(); }
    struct X
    {
        B operator () (signed char, float)                { return B(); }
        C operator () (signed char, float) const          { return C(); }
        D operator () (signed char, float) volatile       { return D(); }
        E operator () (signed char, float) const volatile { return E(); }
    };
}

static_assert(GTFO_RESULT_OF_FUN2_IS(decltype(foo), long int, long double, A), "");
static_assert(GTFO_RESULT_OF_FUN2_IS(X, long int, long double, B), "");
static_assert(GTFO_RESULT_OF_FUN2_IS(X &, long int, long double, B), "");
static_assert(GTFO_RESULT_OF_FUN2_IS(const X, long int, long double, C), "");
static_assert(GTFO_RESULT_OF_FUN2_IS(volatile X, long int, long double, D), "");
static_assert(GTFO_RESULT_OF_FUN2_IS(const volatile X, long int, long double, E), "");
