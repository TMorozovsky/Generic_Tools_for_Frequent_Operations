#include "gtfo/algorithm/find.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

GTFO_TEST_FUN_BEGIN
    const int arr[] = { 1, 3, 5, 3, 1 };

    // two iterators + value
    {
        auto it_result = gtfo::find(begin(arr), end(arr), 2);
        GTFO_TEST_ASSERT(it_result == end(arr))
    }
    {
        auto it_result = gtfo::find(begin(arr), end(arr), 3);
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(it_result - begin(arr), 1)
    }

    // range + value
    {
        auto it_result = gtfo::find(arr, 2);
        GTFO_TEST_ASSERT(it_result == end(arr))
    }
    {
        auto it_result = gtfo::find(arr, 3);
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(it_result - begin(arr), 1)
    }
    {
        auto it_result = gtfo::find(rev(arr), 2);
        GTFO_TEST_ASSERT(it_result == rend(arr))
    }
    {
        auto it_result = gtfo::find(rev(arr), 3);
        GTFO_TEST_ASSERT(it_result != rend(arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(it_result - rbegin(arr), 1)
        GTFO_TEST_ASSERT_EQ(&*it_result - arr, 3)
    }
GTFO_TEST_FUN_END
