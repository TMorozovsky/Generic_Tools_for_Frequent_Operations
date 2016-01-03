#include "gtfo/_impl/type_traits/is_invokable_fun0.hpp"
#define GTFO_IS_INVOKABLE_FUN0(f) ::gtfo::_tt::is_invokable_fun0<f>::value

namespace
{
    inline void foo() { }
    struct bar { void operator()() { } };
    struct baz { private: void operator()() { } };
}

static_assert(!GTFO_IS_INVOKABLE_FUN0(void), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(void (void)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(void(*)(void)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(void(&)(void)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(decltype(foo)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(bar), "");
#ifndef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC // a bug in VC 2012
static_assert(!GTFO_IS_INVOKABLE_FUN0(baz), "");
#endif
