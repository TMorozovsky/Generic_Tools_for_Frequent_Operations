#include "gtfo/_impl/type_traits/mem_fun_invoke_is_callable.hpp"
#define GTFO_IS_MEM_FUN_INVOKE_CALLABLE_0(f)     ::gtfo::_tt::mem_fun_invoke_is_callable<f>::value
#define GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(f,a)   ::gtfo::_tt::mem_fun_invoke_is_callable<f, a>::value
#define GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(f,a,b) ::gtfo::_tt::mem_fun_invoke_is_callable<f, a, b>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct bar0 { void invoke() { } };
    struct baz0 { private: void invoke() { } };
    struct lval0 { void invoke() & { } };
}

static_assert( GTFO_IS_MEM_FUN_INVOKE_CALLABLE_0(bar0),    "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_0(baz0),    "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_0(lval0),   "");
static_assert( GTFO_IS_MEM_FUN_INVOKE_CALLABLE_0(lval0 &), "");

namespace
{
    struct A1 { };
    struct B1 { };
    struct C1 {          operator A1() const { return A1(); } };
    struct D1 { private: operator A1() const { return A1(); } };

    struct bar1 {          void invoke(A1) { } };
    struct baz1 { private: void invoke(A1) { } };
}

static_assert( GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(bar1, A1), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(bar1, B1), "");
static_assert( GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(bar1, C1), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(bar1, D1), "");

static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(baz1, A1), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(baz1, C1), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_1(baz1, D1), "");

namespace
{
    struct A2 { };
    struct B2 { };
    struct C2 {          operator A2() const { return A2(); } };
    struct D2 { private: operator A2() const { return A2(); } };

    struct bar2 {          void invoke(int, A2) { } };
    struct baz2 { private: void invoke(int, A2) { } };
}

static_assert( GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(bar2, char, A2), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(bar2, char, B2), "");
static_assert( GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(bar2, char, C2), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(bar2, char, D2), "");

static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(baz2, char, A2), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(baz2, char, C2), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_CALLABLE_2(baz2, char, D2), "");
