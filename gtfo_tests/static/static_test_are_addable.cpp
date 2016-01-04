#include "gtfo/_impl/type_traits/are_addable.hpp"
#define GTFO_ARE_ADDABLE(t1,t2) ::gtfo::_tt::are_addable<t1,t2>::value

namespace
{
    struct X { };
    struct Y { };
    inline bool operator + (X, Y) { return true; }
}

static_assert(GTFO_ARE_ADDABLE(int, int), "");
static_assert(GTFO_ARE_ADDABLE(int, int &), "");
static_assert(GTFO_ARE_ADDABLE(int, const volatile int &), "");
static_assert(GTFO_ARE_ADDABLE(int, float), "");
static_assert(GTFO_ARE_ADDABLE(int &, const volatile long double &), "");
static_assert(!GTFO_ARE_ADDABLE(float &, void *), "");

static_assert(GTFO_ARE_ADDABLE(X, Y &), "");
static_assert(GTFO_ARE_ADDABLE(X &, Y), "");
static_assert(GTFO_ARE_ADDABLE(X, Y), "");
static_assert(!GTFO_ARE_ADDABLE(X, X), "");
static_assert(!GTFO_ARE_ADDABLE(Y, X), "");
