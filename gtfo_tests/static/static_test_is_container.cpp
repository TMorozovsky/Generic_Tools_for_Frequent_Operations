#include "gtfo/_impl/type_traits/is_container.hpp"
#define GTFO_HAS_ITERATOR_RETURNING_BEGIN(t) ::gtfo::_tt::helpers::has_iterator_returning_begin<t>::value
#define GTFO_HAS_ITERATOR_RETURNING_END(t) ::gtfo::_tt::helpers::has_iterator_returning_end<t>::value
#define GTFO_IS_CONTAINER(t) ::gtfo::_tt::is_container<t>::value

namespace
{
    struct A { };
    struct B { void begin() { } };
    struct C { void end() { } };
    struct D { int * begin() { return nullptr; } };
    struct E { int * end() { return nullptr; } };
    struct F { int * begin() { return nullptr; } float * end() { return nullptr; } };
    struct G { int * begin() { return nullptr; } int * end() { return nullptr; } };
}

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(A &), "");
static_assert(!GTFO_HAS_ITERATOR_RETURNING_END(A &), "");
static_assert(!GTFO_IS_CONTAINER(A), "");

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(B &), "");
static_assert(!GTFO_HAS_ITERATOR_RETURNING_END(B &), "");
static_assert(!GTFO_IS_CONTAINER(B), "");

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(C &), "");
static_assert(!GTFO_HAS_ITERATOR_RETURNING_END(C &), "");
static_assert(!GTFO_IS_CONTAINER(C), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(D &), "");
static_assert(!GTFO_HAS_ITERATOR_RETURNING_END(D &), "");
static_assert(!GTFO_IS_CONTAINER(D), "");

static_assert(!GTFO_HAS_ITERATOR_RETURNING_BEGIN(E &), "");
static_assert(GTFO_HAS_ITERATOR_RETURNING_END(E &), "");
static_assert(!GTFO_IS_CONTAINER(E), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(F &), "");
static_assert(GTFO_HAS_ITERATOR_RETURNING_END(F &), "");
static_assert(!GTFO_IS_CONTAINER(F), "");

static_assert(GTFO_HAS_ITERATOR_RETURNING_BEGIN(G &), "");
static_assert(GTFO_HAS_ITERATOR_RETURNING_END(G &), "");
static_assert(GTFO_IS_CONTAINER(G), "");
