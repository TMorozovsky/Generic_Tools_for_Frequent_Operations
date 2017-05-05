#include "gtfo/_impl/type_traits/is_incrementable.hpp"
#define GTFO_IS_INCREMENTABLE(t) ::gtfo::_tt::is_incrementable<t>::value

namespace
{
    struct A { };
    struct B { void operator++() { } };
    struct C { private: void operator++() { } };
}

static_assert(!GTFO_IS_INCREMENTABLE(void), "");
static_assert( GTFO_IS_INCREMENTABLE(int), "");
static_assert( GTFO_IS_INCREMENTABLE(int *), "");
static_assert(!GTFO_IS_INCREMENTABLE(int *const), "");
static_assert(!GTFO_IS_INCREMENTABLE(A), "");
static_assert( GTFO_IS_INCREMENTABLE(B), "");
static_assert(!GTFO_IS_INCREMENTABLE(C), "");
