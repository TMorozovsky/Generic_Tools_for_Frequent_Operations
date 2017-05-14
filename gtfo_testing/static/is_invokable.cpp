#include "gtfo/_impl/type_traits/is_invokable.hpp"
#define GTFO_IS_INVOKABLE_0(f)     ::gtfo::_tt::is_invokable<f>::value
#define GTFO_IS_INVOKABLE_1(f,a)   ::gtfo::_tt::is_invokable<f, a>::value
#define GTFO_IS_INVOKABLE_2(f,a,b) ::gtfo::_tt::is_invokable<f, a, b>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    inline UNUSED void foo0() { }
    struct bar0 { void operator()() { } };
    struct baz0 { private: void operator()() { } };
    struct lval0 { void operator()() & { } };
}

static_assert(!GTFO_IS_INVOKABLE_0(void),           "");
static_assert( GTFO_IS_INVOKABLE_0(void (void)),    "");
static_assert( GTFO_IS_INVOKABLE_0(void(*)(void)),  "");
static_assert( GTFO_IS_INVOKABLE_0(void(&)(void)),  "");
static_assert( GTFO_IS_INVOKABLE_0(decltype(foo0)), "");
static_assert( GTFO_IS_INVOKABLE_0(bar0),    "");
static_assert(!GTFO_IS_INVOKABLE_0(baz0),    "");
static_assert(!GTFO_IS_INVOKABLE_0(lval0),   "");
static_assert( GTFO_IS_INVOKABLE_0(lval0 &), "");

namespace
{
    struct A1 { };
    struct B1 { };
    struct C1 { operator A1() const { return A1(); } };
    struct D1 { private: operator A1() const { return A1(); } };

    struct bar1 { void operator()(A1) { } };
    struct baz1 { private: void operator()(A1) { } };
}

static_assert( GTFO_IS_INVOKABLE_1(void (A1),   A1), "");
static_assert( GTFO_IS_INVOKABLE_1(void(&)(A1), A1), "");
static_assert( GTFO_IS_INVOKABLE_1(void(*)(A1), A1), "");
static_assert(!GTFO_IS_INVOKABLE_1(void (A1),   B1), "");
static_assert(!GTFO_IS_INVOKABLE_1(void(&)(A1), B1), "");
static_assert(!GTFO_IS_INVOKABLE_1(void(*)(A1), B1), "");
static_assert( GTFO_IS_INVOKABLE_1(void (A1),   C1), "");
static_assert(!GTFO_IS_INVOKABLE_1(void (A1),   D1), "");

static_assert( GTFO_IS_INVOKABLE_1(bar1, A1), "");
static_assert(!GTFO_IS_INVOKABLE_1(bar1, B1), "");
static_assert( GTFO_IS_INVOKABLE_1(bar1, C1), "");
static_assert(!GTFO_IS_INVOKABLE_1(bar1, D1), "");

static_assert(!GTFO_IS_INVOKABLE_1(baz1, A1), "");
static_assert(!GTFO_IS_INVOKABLE_1(baz1, C1), "");
static_assert(!GTFO_IS_INVOKABLE_1(baz1, D1), "");

namespace
{
    struct A2 { };
    struct B2 { };
    struct C2 { operator A2() const { return A2(); } };
    struct D2 { private: operator A2() const { return A2(); } };

    struct bar2 { void operator()(int, A2) { } };
    struct baz2 { private: void operator()(int, A2) { } };
}

static_assert( GTFO_IS_INVOKABLE_2(void (int, A2), char, A2), "");
static_assert(!GTFO_IS_INVOKABLE_2(void (int, A2), char, B2), "");
static_assert( GTFO_IS_INVOKABLE_2(void (int, A2), char, C2), "");
static_assert(!GTFO_IS_INVOKABLE_2(void (int, A2), char, D2), "");

static_assert( GTFO_IS_INVOKABLE_2(bar2, char, A2), "");
static_assert(!GTFO_IS_INVOKABLE_2(bar2, char, B2), "");
static_assert( GTFO_IS_INVOKABLE_2(bar2, char, C2), "");
static_assert(!GTFO_IS_INVOKABLE_2(bar2, char, D2), "");

static_assert(!GTFO_IS_INVOKABLE_2(baz2, char, A2), "");
static_assert(!GTFO_IS_INVOKABLE_2(baz2, char, C2), "");
static_assert(!GTFO_IS_INVOKABLE_2(baz2, char, D2), "");
