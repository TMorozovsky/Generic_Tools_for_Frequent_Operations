#include "gtfo/numeric/inner_product.hpp"
#include "gtfo/reversed_range.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"
#include <vector>
#include <functional>

using namespace gtfo::runtime_test_helpers;

GTFO_TEST_FUN_BEGIN
    using gtfo::begin;
    using gtfo::end;
    using gtfo::rev;

    const int arr1[] = { 2, 4, 6, 8, 10 };
    const int arr2[] = { 10, 20, 40, 80, 160 };
    const std::vector<int> vec2 = make_vec5(arr2[0], arr2[1], arr2[2], arr2[3], arr2[4]);

    const std::plus<int> plus;
    const std::multiplies<int> mult;

    // two iterators + iterator
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), begin(arr2)),                   2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), begin(vec2)),                   2580)

    // two iterators + iterator + value
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), begin(arr2), 42),               2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), begin(vec2), 42),               2622)

    // two iterators + iterator + value + custom operations
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), begin(arr2), 42, plus, plus),   382)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), begin(vec2), 42, plus, plus),   382)

    // two iterators + range (lvalue or rvalue)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), arr2),                          2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), vec2),                          2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), make_vec5i(1, 2, 3, 4, 5)),     110)

    // two iterators + range + value
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), arr2, 42),                      2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), vec2, 42),                      2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), make_vec5i(1, 2, 3, 4, 5), 42), 152)

    // two iterators + range + value + custom operations
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), arr2, 42, plus, plus),          382)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), vec2, 42, plus, plus),          382)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(begin(arr1), end(arr1), make_vec5i(1, 2, 3, 4, 5), 2, mult, plus), 58320)

    // range (lvalue or rvalue) + iterator
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            begin(vec2)),          2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(arr1),                       begin(arr2)),          1140)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(make_vec5i(2, 4, 6, 8, 10),      begin(arr2)),          2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), begin(vec2)),          1140)

    // range + iterator + value
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            begin(arr2), 42),      2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(arr1),                       begin(vec2), 42),      1182)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(make_vec5i(2, 4, 6, 8, 10),      begin(vec2), 42),      2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), begin(arr2), 42),      1182)

    // range + iterator + value + custom operations
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            begin(vec2), 42, plus, mult), 2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(arr1),                       begin(arr2), 42, plus, mult), 1182)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(make_vec5i(2, 4, 6, 8, 10),      begin(vec2), 42, plus, mult), 2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), begin(arr2), 42, plus, mult), 1182)

    // range (lvalue or rvalue) + range (lvalue or rvalue)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            arr2),                 2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(arr1),                       vec2),                 1140)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(make_vec5i(2, 4, 6, 8, 10),      rev(arr2)),            1140)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), rev(vec2)),            2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            make_vec5i(10, 20, 40, 80, 160)),      2580)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(arr1),                       make_vec5i(10, 20, 40, 80, 160)),      1140)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(make_vec5i(2, 4, 6, 8, 10),      rev(make_vec5i(10, 20, 40, 80, 160))), 1140)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), rev(make_vec5i(10, 20, 40, 80, 160))), 2580)

    // range + range + value
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            arr2,      42),        2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            vec2,      42),        2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            rev(arr2), 42),        1182)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), rev(vec2), 42),        2622)

    // range + range + value + custom operations
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            arr2,      42, plus, mult), 2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            vec2,      42, plus, mult), 2622)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(arr1,                            rev(arr2), 42, plus, mult), 1182)
    GTFO_TEST_ASSERT_EQ(gtfo::inner_product(rev(make_vec5i(2, 4, 6, 8, 10)), rev(vec2), 42, plus, mult), 2622)
GTFO_TEST_FUN_END
