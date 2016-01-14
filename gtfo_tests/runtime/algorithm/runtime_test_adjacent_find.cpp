#include "gtfo/algorithm/adjacent_find.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"
#include <vector>

using namespace gtfo::runtime_test_helpers;

namespace
{
    struct eq
    {
        template<typename Lhs, typename Rhs>
        bool operator()(const Lhs & lhs, const Rhs & rhs) const
        {
            return lhs == rhs;
        }
    };
}

using gtfo::adjacent_find;

GTFO_TEST_FUN_BEGIN
    const int arr[] = { 1, 2, 3, 4, 4, 5, 6, 6, 7, 8, 9 };
    std::vector<int> noadj = make_vec10i(1, 2, 3, 4, 5, 6, 7, 8, 9, 0);

    using gtfo::adjacent_find;

    // two iterators
    {
        auto it_result = adjacent_find(begin(arr), end(arr));
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(it_result - arr, 3)
    }
    {
        auto it_result = ::adjacent_find(begin(noadj), end(noadj));
        GTFO_TEST_ASSERT(it_result == end(noadj))
    }

    // two iterators + binary predicate
    {
        auto it_result = adjacent_find(begin(arr), end(arr), eq());
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(it_result - arr, 3)
    }
    {
        auto it_result = ::adjacent_find(begin(noadj), end(noadj), eq());
        GTFO_TEST_ASSERT(it_result == end(noadj))
    }

    // range
    {
        auto it_result = adjacent_find(arr);
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(it_result - arr, 3)
    }
    {
        auto it_result = adjacent_find(rev(arr));
        GTFO_TEST_ASSERT(it_result != rend(arr))
        GTFO_TEST_ASSERT_EQ(&*it_result - arr, 7)
    }
    {
        auto it_result = ::adjacent_find(noadj);
        GTFO_TEST_ASSERT(it_result == end(noadj))
    }

    // range + binary predicate
    {
        auto it_result = adjacent_find(arr, eq());
        GTFO_TEST_ASSERT(it_result != end(arr))
        GTFO_TEST_ASSERT_EQ(it_result - arr, 3)
    }
    {
        auto it_result = adjacent_find(rev(arr), eq());
        GTFO_TEST_ASSERT(it_result != rend(arr))
        GTFO_TEST_ASSERT_EQ(&*it_result - arr, 7)
    }
    {
        auto it_result = ::adjacent_find(noadj, eq());
        GTFO_TEST_ASSERT(it_result == end(noadj))
    }
GTFO_TEST_FUN_END
