#include "gtfo/_impl/type_traits/mem_fun_invoke_call_result.hpp"
#define GTFO_RESULT_OF_MEM_FUN_INVOKE1(f,a) ::gtfo::_tt::mem_fun_invoke_call_result_t<f,a>
#define GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(f,a,r) ::gtfo::_tt::is_same<GTFO_RESULT_OF_MEM_FUN_INVOKE1(f,a), r>::value

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct E { };
    struct F { };
    struct G { };
    struct H { };

    struct X
    {
        A invoke(signed char)                &  { return A(); }
        B invoke(signed char) const          &  { return B(); }
        C invoke(signed char) volatile       &  { return C(); }
        D invoke(signed char) const volatile &  { return D(); }
        E invoke(signed char)                && { return E(); }
        F invoke(signed char) const          && { return F(); }
        G invoke(signed char) volatile       && { return G(); }
        H invoke(signed char) const volatile && { return H(); }
    };
}

static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(X &,                 unsigned long long, A), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(const X &,           unsigned long long, B), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(volatile X &,        unsigned long long, C), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(const volatile X &,  unsigned long long, D), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(X &&,                unsigned long long, E), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(const X &&,          unsigned long long, F), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(volatile X &&,       unsigned long long, G), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(const volatile X &&, unsigned long long, H), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(X,                   unsigned long long, E), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(const X,             unsigned long long, F), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(volatile X,          unsigned long long, G), "");
static_assert(GTFO_RESULT_OF_MEM_FUN_INVOKE1_IS(const volatile X,    unsigned long long, H), "");
