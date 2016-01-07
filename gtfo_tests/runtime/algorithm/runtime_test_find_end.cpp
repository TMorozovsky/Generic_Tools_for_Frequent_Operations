#include "gtfo/algorithm/find_end.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

namespace
{
    struct is_equal_type
    {
        template<typename Lhs, typename Rhs>
        bool operator()(const Lhs & lhs, const Rhs & rhs) const
        {
            return lhs == rhs;
        }
    };
}

GTFO_TEST_FUN_BEGIN
    const int data_arr[] = { 1, 2, 3, 4, 5, 10, 2, 3, 4, 10 };
    const std::vector<int> data(begin(data_arr), end(data_arr));
    const int sub_true[] = { 2, 3, 4 };
    const int sub_false[] = { 2, 3, 5 };
    const is_equal_type is_equal;

    // two iterators + two iterators
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), begin(sub_false), end(sub_false));
        GTFO_TEST_ASSERT(it_result == data.end())
    }
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), begin(sub_true), end(sub_true));
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(begin(data_arr), end(data_arr), begin(sub_true), end(sub_true));
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }

    // two iterators + two iterators + binary predicate
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), begin(sub_false), end(sub_false), is_equal);
        GTFO_TEST_ASSERT(it_result == data.end())
    }
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), begin(sub_true), end(sub_true), is_equal);
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }

    // two iterators + range
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), sub_true);
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), make_vec3i(2, 3, 4));
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), rev(make_vec3i(4, 3, 2)));
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(begin(data_arr), end(data_arr), sub_true);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }

    // two iterators + range + binary predicate
    {
        auto it_result = gtfo::find_end(data.begin(), data.end(), sub_true, is_equal);
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(begin(data_arr), end(data_arr), sub_true, is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }

    // range + two iterators
    {
        auto it_result = gtfo::find_end(data, begin(sub_true), end(sub_true));
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(data_arr, begin(sub_true), end(sub_true));
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }

    // range + two iterators + binary predicate
    {
        auto it_result = gtfo::find_end(data, begin(sub_true), end(sub_true), is_equal);
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        auto it_result = gtfo::find_end(data_arr, begin(sub_true), end(sub_true), is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }

    // range + range
    {
        auto it_result = gtfo::find_end(data, sub_true);
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        typedef std::vector<int>::const_reverse_iterator crit;
        crit it_result = gtfo::find_end(rev(data), rev(sub_true));
        GTFO_TEST_ASSERT(it_result != data.rend())
        GTFO_TEST_ASSERT_EQ(*it_result, 4)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 3)
    }
    {
        auto it_result = gtfo::find_end(data_arr, sub_true);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }

    // range + range + binary predicate
    {
        auto it_result = gtfo::find_end(data, sub_true, is_equal);
        GTFO_TEST_ASSERT(it_result != data.end())
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 6)
    }
    {
        typedef std::vector<int>::const_reverse_iterator crit;
        crit it_result = gtfo::find_end(rev(data), rev(sub_true), is_equal);
        GTFO_TEST_ASSERT(it_result != data.rend())
        GTFO_TEST_ASSERT_EQ(*it_result, 4)
        GTFO_TEST_ASSERT_EQ(&*it_result - &data[0], 3)
    }
    {
        auto it_result = gtfo::find_end(data_arr, sub_true, is_equal);
        GTFO_TEST_ASSERT(it_result != end(data_arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 2)
        GTFO_TEST_ASSERT_EQ(&*it_result - data_arr, 6)
    }
GTFO_TEST_FUN_END
