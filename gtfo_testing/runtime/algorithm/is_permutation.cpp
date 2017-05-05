#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "gtfo/algorithm/is_permutation.hpp"
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
    const auto vec = make_vec3i(1, 2, 3);
    const int arr[3] = { 3, 1, 2 };
    const is_equal_type is_equal;

    // two iterators + iterator
    {
        bool result = gtfo::is_permutation(vec.begin(), vec.end(), begin(arr));
        GTFO_TEST_ASSERT(result);
    }

    // two iterators + iterator + binary predicate
    {
        bool result = gtfo::is_permutation(vec.begin(), vec.end(), begin(arr), is_equal);
        GTFO_TEST_ASSERT(result);
    }

#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
    // two iterators + two iterators
    {
        bool result = gtfo::is_permutation(vec.begin(), vec.end(), begin(arr), end(arr));
        GTFO_TEST_ASSERT(result);
    }

    // two iterators + two iterators + binary predicate
    {
        bool result = gtfo::is_permutation(vec.begin(), vec.end(), begin(arr), end(arr), is_equal);
        GTFO_TEST_ASSERT(result);
    }
#endif

    // two iterators + range
    {
        bool result = gtfo::is_permutation(vec.begin(), vec.end(), arr);
        GTFO_TEST_ASSERT(result);
    }

    // two iterators + range + binary predicate
    {
        bool result = gtfo::is_permutation(vec.begin(), vec.end(), arr, is_equal);
        GTFO_TEST_ASSERT(result);
    }

    // range + iterator
    {
        bool result = gtfo::is_permutation(vec, begin(arr));
        GTFO_TEST_ASSERT(result);
    }

    // range + iterator + binary predicate
    {
        bool result = gtfo::is_permutation(vec, begin(arr), is_equal);
        GTFO_TEST_ASSERT(result);
    }

#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
    // range + two iterators
    {
        bool result = gtfo::is_permutation(vec, begin(arr), end(arr));
        GTFO_TEST_ASSERT(result);
    }

    // range + two iterators + binary predicate
    {
        bool result = gtfo::is_permutation(vec, begin(arr), end(arr), is_equal);
        GTFO_TEST_ASSERT(result);
    }
#endif

    // range + range
    {
        bool result = gtfo::is_permutation(vec, arr);
        GTFO_TEST_ASSERT(result);
    }

    // range + range + binary predicate
    {
        bool result = gtfo::is_permutation(vec, arr, is_equal);
        GTFO_TEST_ASSERT(result);
    }

GTFO_TEST_FUN_END
