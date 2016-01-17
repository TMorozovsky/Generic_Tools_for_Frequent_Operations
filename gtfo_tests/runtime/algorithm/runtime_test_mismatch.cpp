#include "gtfo/algorithm/mismatch.hpp"
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
    const std::vector<int> data = make_vec5i(1, 2, 3, 4, 10);
    const int sub_1[] = { 1, 2, 3, 4, 5 };
    const is_equal_type is_equal;

    // two iterators + iterator
    {
        auto result_pair = gtfo::mismatch(data.begin(), data.end(), begin(sub_1));
        GTFO_TEST_ASSERT(result_pair.first != data.end());
        GTFO_TEST_ASSERT_EQ(*result_pair.first, 10)
        GTFO_TEST_ASSERT_EQ(*result_pair.second, 5)
    }

    // two iterators + iterator + binary predicate
    {
        auto result_pair = gtfo::mismatch(data.begin(), data.end(), begin(sub_1), is_equal);
        GTFO_TEST_ASSERT(result_pair.first != data.end());
        GTFO_TEST_ASSERT_EQ(*result_pair.first, 10)
        GTFO_TEST_ASSERT_EQ(*result_pair.second, 5)
    }

#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
    // two iterators + two iterators
    {
        auto result_pair = gtfo::mismatch(data.begin(), data.end(), begin(sub_1), end(sub_1));
        GTFO_TEST_ASSERT(result_pair.first != data.end());
        GTFO_TEST_ASSERT_EQ(*result_pair.first, 10)
        GTFO_TEST_ASSERT_EQ(*result_pair.second, 5)
    }

    // two iterators + two iterators + binary predicate
    {
        auto result_pair = gtfo::mismatch(data.begin(), data.end(), begin(sub_1), end(sub_1), is_equal);
        GTFO_TEST_ASSERT(result_pair.first != data.end());
        GTFO_TEST_ASSERT_EQ(*result_pair.first, 10)
        GTFO_TEST_ASSERT_EQ(*result_pair.second, 5)
    }
#endif

    // two iterators + range
    {
        auto result_pair = gtfo::mismatch(data.begin(), data.end(), sub_1);
        GTFO_TEST_ASSERT(result_pair.first != data.end());
        GTFO_TEST_ASSERT_EQ(*result_pair.first, 10)
        GTFO_TEST_ASSERT_EQ(*result_pair.second, 5)
    }

    // two iterators + range + binary predicate
    {
        auto result_pair = gtfo::mismatch(data.begin(), data.end(), sub_1, is_equal);
        GTFO_TEST_ASSERT(result_pair.first != data.end());
        GTFO_TEST_ASSERT_EQ(*result_pair.first, 10)
        GTFO_TEST_ASSERT_EQ(*result_pair.second, 5)
    }

GTFO_TEST_FUN_END
