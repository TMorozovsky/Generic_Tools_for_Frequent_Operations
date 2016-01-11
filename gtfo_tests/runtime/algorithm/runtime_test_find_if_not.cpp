#include "gtfo/algorithm/find_if_not.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

namespace
{
    inline bool is_not_two(int value)
    {
        return value != 2;
    }

    struct is_not_three_type
    {
        bool operator()(int value) const
        {
            return value != 3;
        }
    };

    namespace adl_testing
    {
        struct adl_test_container
        {
            int data[42];
            adl_test_container() { ::std::fill(data, data + 10, 3); }
        };

        inline const int * begin(const adl_test_container & c)
        {
            return ::gtfo::begin(c.data);
        }

        inline const int * end(const adl_test_container & c)
        {
            return ::gtfo::end(c.data);
        }

        inline bool is_three(int x)
        {
            return x == 3;
        }
    }
}

GTFO_TEST_FUN_BEGIN
    const int arr[] = { 1, 3, 5, 3, 1 };
    const is_not_three_type is_not_three;

    // two iterators + unary predicate
    {
        auto it_result = gtfo::find_if_not(begin(arr), end(arr), is_not_two);
        GTFO_TEST_ASSERT(it_result == end(arr))
    }
    {
        auto it_result = gtfo::find_if_not(begin(arr), end(arr), is_not_three);
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(it_result - begin(arr), 1)
    }

    // range + unary predicate
    {
        auto it_result = gtfo::find_if_not(arr, is_not_two);
        GTFO_TEST_ASSERT(it_result == end(arr))
    }
    {
        auto it_result = gtfo::find_if_not(arr, is_not_three);
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(it_result - begin(arr), 1)
    }
    {
        auto it_result = gtfo::find_if_not(rev(arr), is_not_two);
        GTFO_TEST_ASSERT(it_result == rend(arr))
    }
    {
        auto it_result = gtfo::find_if_not(rev(arr), is_not_three);
        GTFO_TEST_ASSERT(it_result != rend(arr))
        GTFO_TEST_ASSERT_EQ(*it_result, 3)
        GTFO_TEST_ASSERT_EQ(it_result - rbegin(arr), 1)
        GTFO_TEST_ASSERT_EQ(&*it_result - arr, 3)
    }

    // extra: test for a container that relies on ADL
    {
        adl_testing::adl_test_container adl_test;
        auto it_result = gtfo::find_if_not(adl_test, adl_testing::is_three);
        GTFO_TEST_ASSERT_EQ(it_result, begin(adl_test) + 10);
    }
GTFO_TEST_FUN_END
