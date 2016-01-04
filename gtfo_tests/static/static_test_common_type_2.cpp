#include "gtfo/_impl/type_traits/common_type_2.hpp"
#define GTFO_COMMON_TYPE_2_IS(t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        typename ::gtfo::_tt::common_type_2<t1,t2>::type, \
        t3 \
    >::value

static_assert(GTFO_COMMON_TYPE_2_IS(const volatile int &, int &, int), "");
static_assert(GTFO_COMMON_TYPE_2_IS(int &, float &, float), "");
static_assert(GTFO_COMMON_TYPE_2_IS(int, float, float), "");
static_assert(GTFO_COMMON_TYPE_2_IS(float, int, float), "");
static_assert(GTFO_COMMON_TYPE_2_IS(int, double, double), "");
static_assert(GTFO_COMMON_TYPE_2_IS(long double &, const double &, long double), "");

namespace
{
    struct X { operator int() const { return 42; } };
    struct Y { Y(int) { } };

    struct Base { };
    struct Derived : Base { };
}


static_assert(GTFO_COMMON_TYPE_2_IS(int, X, int), "");
static_assert(GTFO_COMMON_TYPE_2_IS(int, Y, Y), "");
static_assert(GTFO_COMMON_TYPE_2_IS(Base, Derived, Base), "");
