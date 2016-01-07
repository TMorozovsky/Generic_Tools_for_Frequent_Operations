#include "gtfo/algorithm/all_of.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

namespace
{
    inline bool greater_than_zero(int val)
    {
        return val > 0;
    }
}

GTFO_TEST_FUN_BEGIN
    const int arr_bad[] = { 1, 3, 5, -2, 7 };
    const int arr_good[] = { 2, 3, 5, 7, 11 };

    // two iterators + unary predicate
    GTFO_TEST_ASSERT(!gtfo::all_of(begin(arr_bad), end(arr_bad), greater_than_zero))
    GTFO_TEST_ASSERT(gtfo::all_of(begin(arr_good), end(arr_good), greater_than_zero))

    // range + unary predicate
    GTFO_TEST_ASSERT(!gtfo::all_of(arr_bad, greater_than_zero))
    GTFO_TEST_ASSERT(gtfo::all_of(arr_good, greater_than_zero))
    GTFO_TEST_ASSERT(!gtfo::all_of(rev(arr_bad), greater_than_zero))
    GTFO_TEST_ASSERT(gtfo::all_of(rev(arr_good), greater_than_zero))
    GTFO_TEST_ASSERT(!gtfo::all_of(make_vec5i(1, 2, 3, 4, -5), greater_than_zero))
    GTFO_TEST_ASSERT(gtfo::all_of(rev(make_vec5i(1, 2, 3, 4, 5)), greater_than_zero))
GTFO_TEST_FUN_END
