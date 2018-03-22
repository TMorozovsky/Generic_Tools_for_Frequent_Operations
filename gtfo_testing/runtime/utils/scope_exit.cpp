#include "gtfo_testing/runtime/runtime_tests.hpp"
#include "gtfo/utils/scope_exit.hpp"

#define SCOPE_EXIT GTFO_SCOPE_EXIT

GTFO_TEST_FUN_BEGIN
{
    int x = 0;

    {
        SCOPE_EXIT
        {
            x = 1;
        };
    }

    GTFO_TEST_ASSERT_EQ(x, 1);
}
GTFO_TEST_FUN_END