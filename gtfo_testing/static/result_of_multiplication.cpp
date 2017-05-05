#include "gtfo/_impl/type_traits/result_of_multiplication.hpp"
#define GTFO_RESULT_OF_MULTIPLICATION(x,y) typename ::gtfo::_tt::result_of_multiplication<x,y>::type
#define GTFO_RESULT_OF_MULTIPLICATION_IS(x,y,z) ::gtfo::_tt::is_same<GTFO_RESULT_OF_MULTIPLICATION(x,y), z>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct X { };
    struct Y { };
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    inline UNUSED A operator * (const X &,          const Y &)          { return A(); }
    inline UNUSED B operator * (const volatile X &, const Y &)          { return B(); }
    inline UNUSED C operator * (const X &,          const volatile Y &) { return C(); }
    inline UNUSED D operator * (const volatile X &, const volatile Y &) { return D(); }
}

static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(int, int, int), "");
static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(int, long, long), "");
static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(long, int, long), "");
static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(X, Y, A), "");
static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(volatile X, Y &, B), "");
static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(const X, volatile Y, C), "");
static_assert(GTFO_RESULT_OF_MULTIPLICATION_IS(volatile X, const volatile Y, D), "");
