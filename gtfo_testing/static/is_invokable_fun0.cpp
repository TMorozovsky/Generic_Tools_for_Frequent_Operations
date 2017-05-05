#include "gtfo/_impl/type_traits/is_invokable_fun0.hpp"
#define GTFO_IS_INVOKABLE_FUN0(f) ::gtfo::_tt::is_invokable_fun0<f>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    inline UNUSED void foo() { }
    struct bar { void operator()() { } };
    struct baz { private: void operator()() { } };
}

static_assert(!GTFO_IS_INVOKABLE_FUN0(void), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(void (void)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(void(*)(void)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(void(&)(void)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(decltype(foo)), "");
static_assert( GTFO_IS_INVOKABLE_FUN0(bar), "");
static_assert(!GTFO_IS_INVOKABLE_FUN0(baz), "");
