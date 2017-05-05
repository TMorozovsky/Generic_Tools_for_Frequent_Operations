#include "gtfo/algorithm/find_first_of.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

namespace
{
    struct is_equal_type
    {
        constexpr is_equal_type() noexcept { }

        template<typename Lhs, typename Rhs>
        bool operator()(const Lhs & lhs, const Rhs & rhs) const
        {
            return lhs == rhs;
        }
    };
}

GTFO_TEST_FUN_BEGIN
    const int data_arr[] = { 1, 3, 5, 3, 1 };
    const std::vector<int> data(begin(data_arr), end(data_arr));
    const int set_bad[] = { 7, 8, 9 };
    const int set_good[] = { 2, 3, 4 };
    const is_equal_type is_equal;

    // two iterators + two iterators
    {
        auto it_result = gtfo::find_first_of(begin(data_arr), end(data_arr), begin(set_bad), end(set_bad));
        GTFO_TEST_ASSERT(it_result == end(data_arr))
    }
    {
        auto it_result = gtfo::find_first_of(begin(data_arr), end(data_arr), begin(set_good), end(set_good));
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }

    // two iterators + two iterators + binary predicate
    {
        auto it_result = gtfo::find_first_of(begin(data_arr), end(data_arr), rbegin(set_bad), rend(set_bad), is_equal);
        GTFO_TEST_ASSERT(it_result == end(data_arr))
    }
    {
        auto it_result = gtfo::find_first_of(begin(data_arr), end(data_arr), rbegin(set_good), rend(set_good), is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }

    // two iterators + range
    {
        auto it_result = gtfo::find_first_of(begin(data_arr), end(data_arr), set_good);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }

    // two iterators + range + binary predicate
    {
        auto it_result = gtfo::find_first_of(begin(data_arr), end(data_arr), set_good, is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }

    // range + two iterators
    {
        auto it_result = gtfo::find_first_of(data_arr, begin(set_good), end(set_good));
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }

    // range + two iterators + binary predicate
    {
        auto it_result = gtfo::find_first_of(data_arr, begin(set_good), end(set_good), is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }


    // range + range
    {
        auto it_result = gtfo::find_first_of(data_arr, set_good);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }
    {
        auto it_result = gtfo::find_first_of(rev(data_arr), set_good);
        GTFO_TEST_ASSERT(it_result != rend(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 3)
    }
    {
        auto it_result = gtfo::find_first_of(rev(data), set_good);
        GTFO_TEST_ASSERT(it_result != data.rend())
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 3)
    }

    // range + range + binary predicate
    {
        auto it_result = gtfo::find_first_of(data_arr, rev(set_good), is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 1)
    }
    {
        auto it_result = gtfo::find_first_of(rev(data_arr), set_good);
        GTFO_TEST_ASSERT(it_result != rend(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 3)
    }
    {
        auto it_result = gtfo::find_first_of(data, rev(std::vector<int>(begin(set_good), end(set_good))));
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 1)
    }
GTFO_TEST_FUN_END
