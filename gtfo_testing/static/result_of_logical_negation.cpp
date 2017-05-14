#include "gtfo/_impl/type_traits/result_of_logical_negation.hpp"
#define GTFO_RESULT_OF_LOGICAL_NEGATION(t)      ::gtfo::_tt::result_of_logical_negation_t<t>
#define GTFO_RESULT_OF_LOGICAL_NEGATION_IS(t,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_LOGICAL_NEGATION(t), r>::value

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct X
    {
        A operator ! ()                { return A(); }
        B operator ! () const          { return B(); }
        C operator ! () volatile       { return C(); }
        D operator ! () const volatile { return D(); }
    };
}

static_assert(GTFO_RESULT_OF_LOGICAL_NEGATION_IS(X, A),                "");
static_assert(GTFO_RESULT_OF_LOGICAL_NEGATION_IS(X &, A),              "");
static_assert(GTFO_RESULT_OF_LOGICAL_NEGATION_IS(const X, B),          "");
static_assert(GTFO_RESULT_OF_LOGICAL_NEGATION_IS(volatile X, C),       "");
static_assert(GTFO_RESULT_OF_LOGICAL_NEGATION_IS(const volatile X, D), "");
