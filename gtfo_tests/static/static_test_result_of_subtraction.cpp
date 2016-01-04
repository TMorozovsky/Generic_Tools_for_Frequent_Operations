#include "gtfo/_impl/type_traits/result_of_subtraction.hpp"
#define GTFO_RESULT_OF_SUBTRACTION(x,y) typename ::gtfo::_tt::result_of_subtraction<x,y>::type
#define GTFO_RESULT_OF_SUBTRACTION_IS(x,y,z) ::gtfo::_tt::is_same<GTFO_RESULT_OF_SUBTRACTION(x,y), z>::value

namespace
{
    struct X { };
    struct Y { };
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    inline A operator - (const X &,          const Y &)          { return A(); }
    inline B operator - (const volatile X &, const Y &)          { return B(); }
    inline C operator - (const X &,          const volatile Y &) { return C(); }
    inline D operator - (const volatile X &, const volatile Y &) { return D(); }
}

static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(int, int, int), "");
static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(int, long, long), "");
static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(long, int, long), "");
static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(X, Y, A), "");
static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(volatile X, Y &, B), "");
static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(const X, volatile Y, C), "");
static_assert(GTFO_RESULT_OF_SUBTRACTION_IS(volatile X, const volatile Y, D), "");
