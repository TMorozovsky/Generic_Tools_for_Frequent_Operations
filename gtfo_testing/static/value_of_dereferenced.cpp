#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"
#include <vector>
#define GTFO_VALUE_OF_DEREFERENCED(t) typename ::gtfo::_tt::value_of_dereferenced<t>::type
#define GTFO_VALUE_OF_DEREFERENCED_IS(t,v) ::gtfo::_tt::is_same<GTFO_VALUE_OF_DEREFERENCED(t), v>::value

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct I
    {
        const volatile A & operator * ()                { static const volatile A _{}; return _; }
        const volatile B & operator * () const          { static const volatile B _{}; return _; }
        const volatile C & operator * () volatile       { static const volatile C _{}; return _; }
        const volatile D & operator * () const volatile { static const volatile D _{}; return _; }
    };

    typedef ::std::vector<double> V;
    typedef V::iterator V_it;
    typedef V::const_iterator V_cit;
    typedef V::reverse_iterator V_rit;
    typedef V::const_reverse_iterator V_crit;
}

static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(I, A), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(I &, A), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(const I, B), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(volatile I, C), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(const volatile I, D), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(V_it, double), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(V_cit, double), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(V_rit, double), "");
static_assert(GTFO_VALUE_OF_DEREFERENCED_IS(V_crit, double), "");
