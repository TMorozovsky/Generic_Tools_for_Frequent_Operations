#include "gtfo/_impl/type_traits/are_addable.hpp"
#include "gtfo/_impl/type_traits/is_compound_addable.hpp"
#define GTFO_ARE_ADDABLE(t1,t2)         ::gtfo::_tt::are_addable<t1,t2>::value
#define GTFO_IS_COMPOUND_ADDABLE(t1,t2) ::gtfo::_tt::is_compound_addable<t1,t2>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct X { };
    struct Y { };
    inline UNUSED bool operator +  (X, Y)   { return true; }
    inline UNUSED bool operator += (X, Y)   { return true; }
    inline UNUSED bool operator += (Y &, X) { return true; }
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

static_assert(!GTFO_IS_COMPOUND_ADDABLE(int, void), "");
static_assert( GTFO_IS_COMPOUND_ADDABLE(int, int), "");
static_assert( GTFO_IS_COMPOUND_ADDABLE(X, Y), "");
static_assert( GTFO_IS_COMPOUND_ADDABLE(Y, X), "");
