#include "gtfo/_impl/type_traits/is_invokable_fun1.hpp"
#define GTFO_IS_INVOKABLE_FUN1(f,x) ::gtfo::_tt::is_invokable_fun1<f,x>::value

namespace
{
    struct A { };
    struct B { };
    struct C { operator A() const { return A(); } };
    struct D { private: operator A() const { return A(); } };

    struct bar { void operator()(A) { } };
    struct baz { private: void operator()(A) { } };
}

static_assert( GTFO_IS_INVOKABLE_FUN1(void (A), A), "");
static_assert(!GTFO_IS_INVOKABLE_FUN1(void (A), B), "");
static_assert( GTFO_IS_INVOKABLE_FUN1(void (A), C), "");
#ifndef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC // a bug in VC 2012
static_assert(!GTFO_IS_INVOKABLE_FUN1(void (A), D), "");
#endif

static_assert( GTFO_IS_INVOKABLE_FUN1(bar, A), "");
static_assert(!GTFO_IS_INVOKABLE_FUN1(bar, B), "");
static_assert( GTFO_IS_INVOKABLE_FUN1(bar, C), "");
#ifndef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC // a bug in VC 2012
static_assert(!GTFO_IS_INVOKABLE_FUN1(bar, D), "");
#endif

#ifndef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC // a bug in VC 2012
static_assert(!GTFO_IS_INVOKABLE_FUN1(baz, A), "");
static_assert(!GTFO_IS_INVOKABLE_FUN1(baz, C), "");
static_assert(!GTFO_IS_INVOKABLE_FUN1(baz, D), "");
#endif
