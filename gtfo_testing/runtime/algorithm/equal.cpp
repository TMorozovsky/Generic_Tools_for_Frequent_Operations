#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "gtfo/algorithm/equal.hpp"
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
    const std::vector<int> data = make_vec5i(1, 2, 3, 4, 5);
    const int arr_bad[] = { 1, 2, 3, 4, 10 };
    const int arr_good[] = { 1, 2, 3, 4, 5 };
    const is_equal_type is_equal;

    // two iterators + iterator
    GTFO_TEST_ASSERT(!gtfo::equal(data.begin(), data.end(), begin(arr_bad)))
    GTFO_TEST_ASSERT(gtfo::equal(data.begin(), data.end(), begin(arr_good)))

    // two iterators + iterator + binary predicate
    GTFO_TEST_ASSERT(!gtfo::equal(data.begin(), data.end(), begin(arr_bad), is_equal))
    GTFO_TEST_ASSERT(gtfo::equal(data.begin(), data.end(), begin(arr_good), is_equal))

#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
    // two iterators + two iterators
    GTFO_TEST_ASSERT(!gtfo::equal(data.begin(), data.end(), begin(arr_bad), end(arr_bad)))
    GTFO_TEST_ASSERT(gtfo::equal(data.begin(), data.end(), begin(arr_good), end(arr_good)))

    // two iterators + two iterators + binary predicate
    GTFO_TEST_ASSERT(!gtfo::equal(data.begin(), data.end(), begin(arr_bad), end(arr_bad), is_equal))
    GTFO_TEST_ASSERT(gtfo::equal(data.begin(), data.end(), begin(arr_good), end(arr_good), is_equal))
#endif

    // two iterators + range
    GTFO_TEST_ASSERT(!gtfo::equal(data.begin(), data.end(), arr_bad))
    GTFO_TEST_ASSERT(gtfo::equal(data.begin(), data.end(), arr_good))

    // two iterators + range + binary predicate
    GTFO_TEST_ASSERT(!gtfo::equal(data.begin(), data.end(), arr_bad, is_equal))
    GTFO_TEST_ASSERT(gtfo::equal(data.begin(), data.end(), arr_good, is_equal))

    // range + iterator
    GTFO_TEST_ASSERT(!gtfo::equal(data, begin(arr_bad)))
    GTFO_TEST_ASSERT(gtfo::equal(data, begin(arr_good)))

    // range + iterator + binary predicate
    GTFO_TEST_ASSERT(!gtfo::equal(data, begin(arr_bad), is_equal))
    GTFO_TEST_ASSERT(gtfo::equal(data, begin(arr_good), is_equal))

#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
    // range + two iterators
    GTFO_TEST_ASSERT(!gtfo::equal(data, begin(arr_bad), end(arr_bad)))
    GTFO_TEST_ASSERT(gtfo::equal(data, begin(arr_good), end(arr_good)))

    // range + two iterators + binary predicate
    GTFO_TEST_ASSERT(!gtfo::equal(data, begin(arr_bad), end(arr_bad), is_equal))
    GTFO_TEST_ASSERT(gtfo::equal(data, begin(arr_good), end(arr_good), is_equal))
#endif

    // range + range
    GTFO_TEST_ASSERT(!gtfo::equal(data, arr_bad))
    GTFO_TEST_ASSERT(gtfo::equal(data, arr_good))

    // range + range + binary predicate
    GTFO_TEST_ASSERT(!gtfo::equal(data, arr_bad, is_equal))
    GTFO_TEST_ASSERT(gtfo::equal(data, arr_good, is_equal))

GTFO_TEST_FUN_END
