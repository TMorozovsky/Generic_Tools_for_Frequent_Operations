#include "gtfo/algorithm/search_n.hpp"
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
    const auto vec = make_vec10i(1, 3, 5, 5, 5, 8, 9, 10, 10, 10);
    const is_equal_type is_equal;

    // two iterators + size + value
    {
        auto result = gtfo::search_n(vec.begin(), vec.end(), 3, 5);
        GTFO_TEST_ASSERT_EQ(result - vec.begin(), 2);
    }

    // two iterators + size + value + binary predicate
    {
        auto result = gtfo::search_n(vec.begin(), vec.end(), 3, 5, is_equal);
        GTFO_TEST_ASSERT_EQ(result - vec.begin(), 2);
    }

    // range + size + value
    {
        auto result = gtfo::search_n(vec, 3, 5);
        GTFO_TEST_ASSERT_EQ(result - vec.begin(), 2);
    }

    // range + size + value + binary predicate
    {
        auto result = gtfo::search_n(vec, 3, 5, is_equal);
        GTFO_TEST_ASSERT_EQ(result - vec.begin(), 2);
    }
GTFO_TEST_FUN_END
