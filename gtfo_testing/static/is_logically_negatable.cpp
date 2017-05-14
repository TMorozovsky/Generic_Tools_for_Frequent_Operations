#include "gtfo/_impl/type_traits/is_logically_negatable.hpp"
#define GTFO_IS_LOGICALLY_NEGATABLE(t) ::gtfo::_tt::is_logically_negatable<t>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct A { void operator ! () { } };
    struct B { private: void operator ! () { } };
    struct C { void operator ! () & { } };
}

static_assert(!GTFO_IS_LOGICALLY_NEGATABLE(void),         "");
static_assert(!GTFO_IS_LOGICALLY_NEGATABLE(const void),   "");
static_assert( GTFO_IS_LOGICALLY_NEGATABLE(const void *), "");
static_assert( GTFO_IS_LOGICALLY_NEGATABLE(A),            "");
static_assert(!GTFO_IS_LOGICALLY_NEGATABLE(B),            "");
static_assert(!GTFO_IS_LOGICALLY_NEGATABLE(C),            "");
static_assert( GTFO_IS_LOGICALLY_NEGATABLE(C &),          "");
