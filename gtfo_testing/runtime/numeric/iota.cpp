#include "gtfo/numeric/iota.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"
#include <vector>

using namespace gtfo::runtime_test_helpers;

GTFO_TEST_FUN_BEGIN
    std::vector<int> vi(5);
    int arr[5];

    // two iterators
    gtfo::iota(vi.begin(), vi.end());
    GTFO_TEST_ASSERT_EQ(vi, make_vec5i(0, 1, 2, 3, 4))
    gtfo::iota(begin(arr), end(arr));
    GTFO_TEST_ASSERT_EQ(arr, make_vec5i(0, 1, 2, 3, 4))

    // two iterators + value
    gtfo::iota(vi.begin(), vi.end(), 1);
    GTFO_TEST_ASSERT_EQ(vi, make_vec5i(1, 2, 3, 4, 5))
    gtfo::iota(begin(arr), end(arr), 2);
    GTFO_TEST_ASSERT_EQ(arr, make_vec5i(2, 3, 4, 5, 6))

    // range
    gtfo::iota(vi);
    GTFO_TEST_ASSERT_EQ(vi, make_vec5i(0, 1, 2, 3, 4))
    gtfo::iota(arr);
    GTFO_TEST_ASSERT_EQ(arr, make_vec5i(0, 1, 2, 3, 4))
    gtfo::iota(rev(vi));
    GTFO_TEST_ASSERT_EQ(vi, make_vec5i(4, 3, 2, 1, 0))
    gtfo::iota(rev(arr));
    GTFO_TEST_ASSERT_EQ(arr, make_vec5i(4, 3, 2, 1, 0))

    // range + value
    gtfo::iota(vi, 10);
    GTFO_TEST_ASSERT_EQ(vi, make_vec5i(10, 11, 12, 13, 14))
    gtfo::iota(arr, 20);
    GTFO_TEST_ASSERT_EQ(arr, make_vec5i(20, 21, 22, 23, 24))
    gtfo::iota(rev(vi), 30);
    GTFO_TEST_ASSERT_EQ(vi, make_vec5i(34, 33, 32, 31, 30))
    gtfo::iota(rev(arr), 40);
    GTFO_TEST_ASSERT_EQ(arr, make_vec5i(44, 43, 42, 41, 40))
GTFO_TEST_FUN_END
