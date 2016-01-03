#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include <vector>
#include "gtfo/reversed_range.hpp"
#define GTFO_ITERATOR_OF_RANGE(t) typename ::gtfo::_tt::iterator_of_range<t>::type
#define GTFO_ITERATOR_OF_RANGE_IS(t,i) ::gtfo::_tt::is_same<GTFO_ITERATOR_OF_RANGE(t), i>::value

namespace
{
    struct G
    {
        int * begin() { return nullptr; }
        int * end() { return nullptr; }
        const int * begin() const { return nullptr; }
        const int * end() const { return nullptr; }
    };

    struct HBeginIterator : ::std::vector<signed char>::iterator { };
    struct HEndIterator : ::std::vector<volatile void *>::const_reverse_iterator { };

    inline bool operator == (HBeginIterator, HEndIterator) { return true; }
    inline bool operator != (HBeginIterator, HEndIterator) { return true; }

    struct H
    {
        HBeginIterator begin() { return HBeginIterator(); }
        HEndIterator   end()   { return HEndIterator(); }
    };

    typedef ::std::vector<H> V;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<V &>())) V_lvalue_reversed;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<const V &>())) V_const_lvalue_reversed;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<V>())) V_rvalue_reversed;
    typedef decltype(::gtfo::rev(::gtfo::_tt::declval<const V>())) V_const_rvalue_reversed;
    typedef V::iterator V_it;
    typedef V::const_iterator V_cit;
    typedef V::reverse_iterator V_rit;
    typedef V::const_reverse_iterator V_crit;
}

static_assert(GTFO_ITERATOR_OF_RANGE_IS(G, int *), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(const G, const int *), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(H, HBeginIterator), "");

static_assert(GTFO_ITERATOR_OF_RANGE_IS(double(&)[42], double *), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(V, V_it), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(const V, V_cit), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(V_lvalue_reversed, V_rit), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(V_rvalue_reversed, V_rit), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(V_const_lvalue_reversed, V_crit), "");
static_assert(GTFO_ITERATOR_OF_RANGE_IS(V_const_rvalue_reversed, V_crit), "");
