#include "gtfo/_impl/type_traits/result_of_begin.hpp"
#define GTFO_RESULT_OF_BEGIN(t) typename ::gtfo::_tt::result_of_begin<t>::type
#define GTFO_RESULT_OF_BEGIN_IS(t,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_BEGIN(t), r>::value

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct X
    {
        A begin()                { return A(); }
        B begin() const          { return B(); }
        C begin() volatile       { return C(); }
        D begin() const volatile { return D(); }
    };
}

static_assert(GTFO_RESULT_OF_BEGIN_IS(int(&)[42], int *), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(const int(&)[42], const int *), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(volatile int(&)[42], volatile int *), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(const volatile int(&)[42], const volatile int *), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(X, A), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(X &, A), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(const X, B), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(volatile X, C), "");
static_assert(GTFO_RESULT_OF_BEGIN_IS(const volatile X, D), "");
