#include "gtfo/_impl/type_traits/is_range_with_same_begin_end.hpp"
#include <vector>
#include "gtfo/reversed_range.hpp"
#define GTFO_IS_RANGE_WITH_SAME_BEGIN_END(t) ::gtfo::_tt::is_range_with_same_begin_end<t>::value

namespace
{
    struct E { int begin() { return 0; } int end() { return 0; } };
    struct F { int * begin() { return nullptr; } float * end() { return nullptr; } };
    struct G { int * begin() { return nullptr; } int * end() { return nullptr; } };

    struct HBeginIterator : ::std::vector<char>::iterator { };
    struct HEndIterator { };

    inline bool operator == (HBeginIterator, HEndIterator) { return true; }
    inline bool operator != (HBeginIterator, HEndIterator) { return true; }

    struct H
    {
        HBeginIterator begin() { return HBeginIterator(); }
        HEndIterator   end()   { return HEndIterator(); }
    };

    typedef ::std::vector<G> V;
}

static_assert(!GTFO_IS_RANGE_WITH_SAME_BEGIN_END(E), "");
static_assert(!GTFO_IS_RANGE_WITH_SAME_BEGIN_END(F), "");
static_assert( GTFO_IS_RANGE_WITH_SAME_BEGIN_END(G), "");
static_assert(!GTFO_IS_RANGE_WITH_SAME_BEGIN_END(H), "");

static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(int(&)[42]), "");
static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(decltype(::gtfo::rev(::gtfo::_tt::declval<int(&)[42]>()))), "");
static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(decltype(::gtfo::rev(::gtfo::_tt::declval<int   [42]>()))), "");
static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(decltype(::gtfo::rev(::gtfo::_tt::declval<int(&&)[42]>()))), "");

static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(V), "");
static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(decltype(::gtfo::rev(::gtfo::_tt::declval<V &>()))), "");
static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(decltype(::gtfo::rev(::gtfo::_tt::declval<V  >()))), "");
static_assert(GTFO_IS_RANGE_WITH_SAME_BEGIN_END(decltype(::gtfo::rev(::gtfo::_tt::declval<V &&>()))), "");
