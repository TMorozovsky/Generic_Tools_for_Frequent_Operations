#include "gtfo/_impl/type_traits/are_multipliable.hpp"
#define GTFO_ARE_MULTIPLIABLE(t1,t2) ::gtfo::_tt::are_multipliable<t1,t2>::value

namespace
{
    struct X { };
    struct Y { };
    inline bool operator * (X, Y) { return true; }
}

static_assert(GTFO_ARE_MULTIPLIABLE(int, int), "");
static_assert(GTFO_ARE_MULTIPLIABLE(int, int &), "");
static_assert(GTFO_ARE_MULTIPLIABLE(int, const volatile int &), "");
static_assert(GTFO_ARE_MULTIPLIABLE(int, float), "");
static_assert(GTFO_ARE_MULTIPLIABLE(int &, const volatile long double &), "");
static_assert(!GTFO_ARE_MULTIPLIABLE(float &, void *), "");

static_assert(GTFO_ARE_MULTIPLIABLE(X, Y &), "");
static_assert(GTFO_ARE_MULTIPLIABLE(X &, Y), "");
static_assert(GTFO_ARE_MULTIPLIABLE(X, Y), "");
static_assert(!GTFO_ARE_MULTIPLIABLE(X, X), "");
static_assert(!GTFO_ARE_MULTIPLIABLE(Y, X), "");
