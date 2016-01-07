#include "gtfo/algorithm/any_of.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

namespace
{
    inline bool less_than_zero(int val)
    {
        return val < 0;
    }
}

GTFO_TEST_FUN_BEGIN
    const int arr_bad[] = { 2, 3, 5, 7, 11 };
    const int arr_good[] = { 1, 3, 5, -2, 7 };

    // two iterators + unary predicate
    GTFO_TEST_ASSERT(!gtfo::any_of(begin(arr_bad), end(arr_bad), less_than_zero))
    GTFO_TEST_ASSERT(gtfo::any_of(begin(arr_good), end(arr_good), less_than_zero))

    // range + unary predicate
    GTFO_TEST_ASSERT(!gtfo::any_of(arr_bad, less_than_zero))
    GTFO_TEST_ASSERT(gtfo::any_of(arr_good, less_than_zero))
    GTFO_TEST_ASSERT(!gtfo::any_of(rev(arr_bad), less_than_zero))
    GTFO_TEST_ASSERT(gtfo::any_of(rev(arr_good), less_than_zero))
    GTFO_TEST_ASSERT(!gtfo::any_of(make_vec5i(1, 2, 3, 4, 5), less_than_zero))
    GTFO_TEST_ASSERT(gtfo::any_of(rev(make_vec5i(1, 2, 3, 4, -5)), less_than_zero))
GTFO_TEST_FUN_END
