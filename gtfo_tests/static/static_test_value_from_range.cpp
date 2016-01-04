#include "gtfo/_impl/type_traits/value_from_range.hpp"
#include "gtfo/reversed_range.hpp"
#include <vector>
#define GTFO_VALUE_FROM_RANGE(t) typename ::gtfo::_tt::value_from_range<t>::type
#define GTFO_VALUE_FROM_RANGE_IS(t,v) ::gtfo::_tt::is_same<GTFO_VALUE_FROM_RANGE(t), v>::value

namespace
{
    struct A { };
    struct I
    {
        I & operator ++ () { return *this; }
        A operator * () { return A(); }
        friend inline bool operator == (I, I) { return true; }
        friend inline bool operator != (I, I) { return true; }
    };
    struct I_end { };
    inline bool operator == (I, I_end) { return true; }
    inline bool operator != (I, I_end) { return true; }
    struct X
    {
        I     begin() { return I(); }
        I_end end()   { return I_end(); }
    };

    typedef ::std::vector<double> V;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<V &>())) V_lvalue_reversed;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<const V &>())) V_const_lvalue_reversed;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<V>())) V_rvalue_reversed;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<const V>())) V_const_rvalue_reversed;
    typedef V::iterator V_it;
    typedef V::const_iterator V_cit;
    typedef V::reverse_iterator V_rit;
    typedef V::const_reverse_iterator V_crit;
}

static_assert(GTFO_VALUE_FROM_RANGE_IS(int(&)[42], int), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(const int(&)[42], int), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(volatile int(&)[42], int), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(const volatile int(&)[42], int), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(X, A), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(X &, A), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(V, double), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(V &, double), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(const V, double), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(V_lvalue_reversed, double), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(V_const_lvalue_reversed, double), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(V_rvalue_reversed, double), "");
static_assert(GTFO_VALUE_FROM_RANGE_IS(V_const_rvalue_reversed, double), "");
