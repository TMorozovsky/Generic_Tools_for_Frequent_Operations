#include "gtfo/_impl/type_traits/mem_fun_invoke_is_nothrow_callable.hpp"
#define GTFO_IS_MEM_FUN_INVOKE_NOTHROW_CALLABLE_1(f,a) ::gtfo::_tt::mem_fun_invoke_is_nothrow_callable<f, a>::value

namespace
{
    struct A
    {
        void invoke(int) { }
        void invoke(char) && noexcept { }
        void invoke(char) const & { }
    };

    struct B { };
}

static_assert(!GTFO_IS_MEM_FUN_INVOKE_NOTHROW_CALLABLE_1(A,    unsigned long long), "");
static_assert( GTFO_IS_MEM_FUN_INVOKE_NOTHROW_CALLABLE_1(A &&, char), "");
static_assert( GTFO_IS_MEM_FUN_INVOKE_NOTHROW_CALLABLE_1(A,    char), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_NOTHROW_CALLABLE_1(A &,  char), "");
static_assert(!GTFO_IS_MEM_FUN_INVOKE_NOTHROW_CALLABLE_1(B &,  int),  "");
