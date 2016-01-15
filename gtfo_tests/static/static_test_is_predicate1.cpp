#include "gtfo/_impl/type_traits/is_predicate1.hpp"
#define GTFO_IS_PREDICATE1(p,t) ::gtfo::_tt::is_predicate1<p,t>::value

namespace
{
    struct A { };
    struct B { };

    struct Bool
    {
        operator bool() const { return false; }
    };

    struct Bad
    {
        void operator()(A) const { }
    };

    struct Good
    {
        Bool operator()(A) const { return Bool(); }
    };
}

static_assert(!GTFO_IS_PREDICATE1(Bad, A), "");
static_assert(!GTFO_IS_PREDICATE1(Bad, B), "");
static_assert( GTFO_IS_PREDICATE1(Good, A), "");
static_assert(!GTFO_IS_PREDICATE1(Good, B), "");
static_assert( GTFO_IS_PREDICATE1(int(*)(const A &), A), "");
