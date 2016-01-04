#include "gtfo/_impl/type_traits/are_subtractable.hpp"
#define GTFO_ARE_SUBTRACTABLE(t1,t2) ::gtfo::_tt::are_subtractable<t1,t2>::value

namespace
{
    struct X { };
    struct Y { };
    inline bool operator - (X, Y) { return true; }
}

static_assert(GTFO_ARE_SUBTRACTABLE(int, int), "");
static_assert(GTFO_ARE_SUBTRACTABLE(int, int &), "");
static_assert(GTFO_ARE_SUBTRACTABLE(int, const volatile int &), "");
static_assert(GTFO_ARE_SUBTRACTABLE(int, float), "");
static_assert(GTFO_ARE_SUBTRACTABLE(int &, const volatile long double &), "");
static_assert(!GTFO_ARE_SUBTRACTABLE(float &, void *), "");

static_assert(GTFO_ARE_SUBTRACTABLE(X, Y &), "");
static_assert(GTFO_ARE_SUBTRACTABLE(X &, Y), "");
static_assert(GTFO_ARE_SUBTRACTABLE(X, Y), "");
static_assert(!GTFO_ARE_SUBTRACTABLE(X, X), "");
static_assert(!GTFO_ARE_SUBTRACTABLE(Y, X), "");
