#include "gtfo/_impl/type_traits/is_predicate2.hpp"
#define GTFO_IS_PREDICATE2(p,t,u) ::gtfo::_tt::is_predicate2<p,t,u>::value

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
        void operator()(A, B) const { }
    };

    struct Good
    {
        Bool operator()(A, B) const { return Bool(); }
    };
}

static_assert(!GTFO_IS_PREDICATE2(Bad, A, B), "");
static_assert( GTFO_IS_PREDICATE2(Good, A, B), "");
static_assert(!GTFO_IS_PREDICATE2(Good, A, A), "");
static_assert( GTFO_IS_PREDICATE2(int(*)(A &, B &&), A &, B), "");
