#include "gtfo/_impl/type_traits/is_invokable_fun2.hpp"
#define GTFO_IS_INVOKABLE_FUN2(f,x,y) ::gtfo::_tt::is_invokable_fun2<f,x,y>::value

namespace
{
    struct A { };
    struct B { };
    struct C { operator A() const { return A(); } };
    struct D { private: operator A() const { return A(); } };

    struct bar { void operator()(int, A) { } };
    struct baz { private: void operator()(int, A) { } };
}

static_assert( GTFO_IS_INVOKABLE_FUN2(void (int, A), char, A), "");
static_assert(!GTFO_IS_INVOKABLE_FUN2(void (int, A), char, B), "");
static_assert( GTFO_IS_INVOKABLE_FUN2(void (int, A), char, C), "");
static_assert(!GTFO_IS_INVOKABLE_FUN2(void (int, A), char, D), "");

static_assert( GTFO_IS_INVOKABLE_FUN2(bar, char, A), "");
static_assert(!GTFO_IS_INVOKABLE_FUN2(bar, char, B), "");
static_assert( GTFO_IS_INVOKABLE_FUN2(bar, char, C), "");
static_assert(!GTFO_IS_INVOKABLE_FUN2(bar, char, D), "");

static_assert(!GTFO_IS_INVOKABLE_FUN2(baz, char, A), "");
static_assert(!GTFO_IS_INVOKABLE_FUN2(baz, char, C), "");
static_assert(!GTFO_IS_INVOKABLE_FUN2(baz, char, D), "");
