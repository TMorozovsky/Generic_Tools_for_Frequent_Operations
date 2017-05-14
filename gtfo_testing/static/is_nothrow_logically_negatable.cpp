#include "gtfo/_impl/type_traits/is_nothrow_logically_negatable.hpp"
#define GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(t) ::gtfo::_tt::is_nothrow_logically_negatable<t>::value

namespace
{
    struct A { };
    struct B { private: void operator ! () noexcept { } };
    struct C { void operator ! () noexcept { } };
    struct D { void operator ! () && noexcept { }
               void operator ! () & { } };
}

static_assert(!GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(void),      "");
static_assert( GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(void *),    "");
static_assert( GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(int),       "");
static_assert(!GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(A),         "");
static_assert(!GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(B),         "");
static_assert( GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(C),         "");
static_assert( GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(D),         "");
static_assert(!GTFO_IS_NOTHROW_LOGICALLY_NEGATABLE(const D &), "");
