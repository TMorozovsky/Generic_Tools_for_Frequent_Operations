#include "gtfo/_impl/type_traits/result_of_end.hpp"
#define GTFO_RESULT_OF_END(t) typename ::gtfo::_tt::result_of_end<t>::type
#define GTFO_RESULT_OF_END_IS(t,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_END(t), r>::value

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct X
    {
        A end()                { return A(); }
        B end() const          { return B(); }
        C end() volatile       { return C(); }
        D end() const volatile { return D(); }
    };
}

static_assert(GTFO_RESULT_OF_END_IS(int(&)[42], int *), "");
static_assert(GTFO_RESULT_OF_END_IS(const int(&)[42], const int *), "");
static_assert(GTFO_RESULT_OF_END_IS(volatile int(&)[42], volatile int *), "");
static_assert(GTFO_RESULT_OF_END_IS(const volatile int(&)[42], const volatile int *), "");
static_assert(GTFO_RESULT_OF_END_IS(X, A), "");
static_assert(GTFO_RESULT_OF_END_IS(X &, A), "");
static_assert(GTFO_RESULT_OF_END_IS(const X, B), "");
static_assert(GTFO_RESULT_OF_END_IS(volatile X, C), "");
static_assert(GTFO_RESULT_OF_END_IS(const volatile X, D), "");
