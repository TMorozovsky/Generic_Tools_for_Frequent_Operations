#include "gtfo/_impl/type_traits/is_range.hpp"
#include <vector>
#include "gtfo/reversed_range.hpp"
#define GTFO_HAS_ITERATOR_RETURNING_BEGIN(t) ::gtfo::_tt::helpers::has_iterator_returning_begin<t>::value
#define GTFO_HAS_NON_VOID_RETURNING_END(t) ::gtfo::_tt::helpers::has_non_void_returning_end<t>::value
#define GTFO_IS_RANGE(t) ::gtfo::_tt::is_range<t>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct A { };
    struct B { void begin() { } };
    struct C { void end() { } };
    struct D { int * begin() { return nullptr; } };
    struct E { int begin() { return 0; } int end() { return 0; } };
    struct F { int * begin() { return nullptr; } float * end() { return nullptr; } };
    struct G { int * begin() { return nullptr; } int * end() { return nullptr; } };

    struct HBeginIterator : ::std::vector<A>::iterator { };
    struct HEndIterator { };

    inline UNUSED bool operator == (HBeginIterator, HEndIterator) { return true; }
    inline UNUSED bool operator != (HBeginIterator, HEndIterator) { return true; }

    struct H
    {
        HBeginIterator begin() { return HBeginIterator(); }
        HEndIterator   end()   { return HEndIterator(); }
    };

    typedef ::std::vector<G> V;
}

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(A &), "");
static_assert(!GTFO_HAS_NON_VOID_RETURNING_END(A &), "");
static_assert(!GTFO_IS_RANGE(A), "");

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(B &), "");
static_assert(!GTFO_HAS_NON_VOID_RETURNING_END(B &), "");
static_assert(!GTFO_IS_RANGE(B), "");

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(C &), "");
static_assert(!GTFO_HAS_NON_VOID_RETURNING_END(C &), "");
static_assert(!GTFO_IS_RANGE(C), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(D &), "");
static_assert(!GTFO_HAS_NON_VOID_RETURNING_END(D &), "");
static_assert(!GTFO_IS_RANGE(D), "");

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(E &), "");
static_assert(GTFO_HAS_NON_VOID_RETURNING_END(E &), "");
static_assert(!GTFO_IS_RANGE(E), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(F &), "");
static_assert(GTFO_HAS_NON_VOID_RETURNING_END(F &), "");
static_assert(!GTFO_IS_RANGE(F), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(G &), "");
static_assert(GTFO_HAS_NON_VOID_RETURNING_END(G &), "");
static_assert(GTFO_IS_RANGE(G), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(H &), "");
static_assert(GTFO_HAS_NON_VOID_RETURNING_END(H &), "");
static_assert(GTFO_IS_RANGE(H), "");

static_assert(GTFO_IS_RANGE(int(&)[42]), "");
static_assert(GTFO_IS_RANGE(decltype(::gtfo::rev(::gtfo::_tt::declval<int(&)[42]>()))), "");
static_assert(GTFO_IS_RANGE(decltype(::gtfo::rev(::gtfo::_tt::declval<int   [42]>()))), "");
static_assert(GTFO_IS_RANGE(decltype(::gtfo::rev(::gtfo::_tt::declval<int(&&)[42]>()))), "");

static_assert(GTFO_IS_RANGE(V), "");
static_assert(GTFO_IS_RANGE(decltype(::gtfo::rev(::gtfo::_tt::declval<V &>()))), "");
static_assert(GTFO_IS_RANGE(decltype(::gtfo::rev(::gtfo::_tt::declval<V  >()))), "");
static_assert(GTFO_IS_RANGE(decltype(::gtfo::rev(::gtfo::_tt::declval<V &&>()))), "");
