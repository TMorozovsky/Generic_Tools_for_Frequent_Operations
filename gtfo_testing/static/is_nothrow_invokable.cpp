#include "gtfo/_impl/type_traits/is_nothrow_invokable.hpp"
#define GTFO_IS_NOTHROW_INVOKABLE_1(f,a) ::gtfo::_tt::is_nothrow_invokable<f, a>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    void UNUSED foo(int) { }

    struct A
    {
        void operator()(int) { }
        void operator()(char) && noexcept { }
        void operator()(char) const & { }
    };

    struct B { };
}

static_assert(!GTFO_IS_NOTHROW_INVOKABLE_1(decltype(foo), int), "");
static_assert(!GTFO_IS_NOTHROW_INVOKABLE_1(A,    unsigned long long), "");
static_assert( GTFO_IS_NOTHROW_INVOKABLE_1(A &&, char), "");
static_assert( GTFO_IS_NOTHROW_INVOKABLE_1(A,    char), "");
static_assert(!GTFO_IS_NOTHROW_INVOKABLE_1(A &,  char), "");
static_assert(!GTFO_IS_NOTHROW_INVOKABLE_1(B &,  int),  "");
