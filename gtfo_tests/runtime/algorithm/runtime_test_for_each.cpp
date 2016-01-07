#include "gtfo/algorithm/for_each.hpp"
#include "gtfo/reversed_range.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"
#include <vector>

using namespace gtfo::runtime_test_helpers;

namespace
{
    struct vi_inserter
    {
        std::vector<int> & dest;

        explicit vi_inserter(std::vector<int> & dest) : dest(dest) { }

        void operator()(int i) const
        {
            dest.push_back(i);
        }
    };
}

GTFO_TEST_FUN_BEGIN
    using gtfo::begin;
    using gtfo::end;
    using gtfo::rev;

    std::vector<int> dest;
    vi_inserter ins(dest);

    const int arr[] = { 1, 3, 5, 7, 9 };

    // two iterators + unary function
    dest.clear();
    gtfo::for_each(begin(arr), end(arr), ins);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(1, 3, 5, 7, 9));

    // range (lvalue or rvalue) + unary function
    dest.clear();
    gtfo::for_each(arr, ins);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(1, 3, 5, 7, 9));
    dest.clear();
    gtfo::for_each(rev(arr), ins);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(9, 7, 5, 3, 1));
    dest.clear();
    gtfo::for_each(make_vec5i(10, 20, 30, 40, 50), ins);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(10, 20, 30, 40, 50));
    dest.clear();
    gtfo::for_each(rev(make_vec5i(10, 20, 30, 40, 50)), ins);
    GTFO_TEST_ASSERT_EQ(dest, make_vec5i(50, 40, 30, 20, 10));
GTFO_TEST_FUN_END
