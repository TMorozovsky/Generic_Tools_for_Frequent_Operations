#include "gtfo/numeric/partial_sum.hpp"
#include "gtfo/reversed_range.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"
#include <vector>
#include <functional>

using namespace gtfo::runtime_test_helpers;

GTFO_TEST_FUN_BEGIN
    using gtfo::rev;

    const std::vector<int> vi = make_vec5i(5, 4, 3, 2, 1);
    const int arr[] = { 5, 4, 3, 2, 1 };
    std::vector<int> dest(vi.size());

    const std::multiplies<int> mult;

    // two iterators + output iterator
    gtfo::partial_sum(vi.begin(), vi.end(), dest.begin());
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 9, 12, 14, 15))
    dest.clear();
    gtfo::partial_sum(vi.begin(), vi.end(), std::back_inserter(dest));
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 9, 12, 14, 15))

    // two iterators + output iterator + binary operation
    dest.clear();
    gtfo::partial_sum(vi.begin(), vi.end(), std::back_inserter(dest), mult);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 20, 60, 120, 120))

    // range (lvalue or rvalue) + output iterator
    gtfo::partial_sum(vi, dest.begin());
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 9, 12, 14, 15))
    gtfo::partial_sum(arr, dest.begin());
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 9, 12, 14, 15))
    gtfo::partial_sum(make_vec5i(6, 5, 4, 3, 2), dest.begin());
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(6, 11, 15, 18, 20))
    gtfo::partial_sum(rev(make_vec5i(6, 5, 4, 3, 2)), dest.begin());
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(2, 5, 9, 14, 20))

    // range + output iterator + binary operation
    gtfo::partial_sum(vi, dest.begin(), mult);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 20, 60, 120, 120))
    gtfo::partial_sum(arr, dest.begin(), mult);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(5, 20, 60, 120, 120))
    gtfo::partial_sum(rev(arr), dest.begin(), mult);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(1, 2, 6, 24, 120))
    gtfo::partial_sum(make_vec5i(6, 5, 4, 3, 2), dest.begin(), mult);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(6, 30, 120, 360, 720))
    gtfo::partial_sum(rev(make_vec5i(6, 5, 4, 3, 2)), dest.begin(), mult);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(2, 6, 24, 120, 720))
GTFO_TEST_FUN_END
