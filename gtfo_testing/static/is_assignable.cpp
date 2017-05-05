#include "gtfo/_impl/type_traits/is_assignable.hpp"
#define GTFO_IS_ASSIGNABLE(t1,t2) ::gtfo::_tt::is_assignable<t1,t2>::value

namespace
{
    struct X { void operator = (int) { } };
    struct Y { };
}

static_assert(!GTFO_IS_ASSIGNABLE(void, int), "");
static_assert( GTFO_IS_ASSIGNABLE(int, int), "");
static_assert( GTFO_IS_ASSIGNABLE(int &, int), "");
static_assert( GTFO_IS_ASSIGNABLE(volatile int &, int), "");
static_assert(!GTFO_IS_ASSIGNABLE(const int &, int), "");

static_assert( GTFO_IS_ASSIGNABLE(X, int), "");
static_assert( GTFO_IS_ASSIGNABLE(X &, int), "");
static_assert(!GTFO_IS_ASSIGNABLE(const X &, int), "");
static_assert(!GTFO_IS_ASSIGNABLE(Y, int), "");
static_assert(!GTFO_IS_ASSIGNABLE(Y &, int), "");
static_assert(!GTFO_IS_ASSIGNABLE(const Y &, int), "");
