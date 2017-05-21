#include "gtfo/_impl/type_traits/are_subtractable.hpp"
#include "gtfo/_impl/type_traits/is_compound_subtractable.hpp"
#define GTFO_ARE_SUBTRACTABLE(t1,t2)         ::gtfo::_tt::are_subtractable<t1,t2>::value
#define GTFO_IS_COMPOUND_SUBTRACTABLE(t1,t2) ::gtfo::_tt::is_compound_subtractable<t1,t2>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct X { };
    struct Y { };
    inline UNUSED bool operator -  (X, Y)   { return true; }
    inline UNUSED bool operator -= (X, Y)   { return true; }
    inline UNUSED bool operator -= (Y &, X) { return true; }
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

static_assert(!GTFO_IS_COMPOUND_SUBTRACTABLE(int, void), "");
static_assert( GTFO_IS_COMPOUND_SUBTRACTABLE(int, int), "");
static_assert( GTFO_IS_COMPOUND_SUBTRACTABLE(X, Y), "");
static_assert( GTFO_IS_COMPOUND_SUBTRACTABLE(Y, X), "");
