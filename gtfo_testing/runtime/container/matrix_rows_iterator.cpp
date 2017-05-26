#include "gtfo/_impl/container/matrix/matrix_rows_iterator.hpp"
#include "gtfo/iterator/reverse_iterator.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"
#include <vector>
using gtfo::ptrdiff_t;

static void test_reverse_iterators()
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int * ptr = arr + 4;
    GTFO_TEST_ASSERT_EQ(*ptr, 5);

    gtfo::reverse_iterator<int *> rit { ptr };
    GTFO_TEST_ASSERT_EQ(*rit, 4);
    { auto x = 42 + rit; (void)x; }

    gtfo::reverse_iterator<const int *> crit = rit;
    GTFO_TEST_ASSERT_EQ(*crit, 4);
}

using iterator               = gtfo::detail::container::matrix_rows_iterator              <int *,                               ptrdiff_t>;
using const_iterator         = gtfo::detail::container::matrix_rows_const_iterator        <const int *,                         ptrdiff_t, iterator>;
using reverse_iterator       = gtfo::detail::container::matrix_rows_reverse_iterator      <gtfo::reverse_iterator<int *>,       ptrdiff_t, iterator>;
using const_reverse_iterator = gtfo::detail::container::matrix_rows_const_reverse_iterator<gtfo::reverse_iterator<const int *>, ptrdiff_t, reverse_iterator, const_iterator>;

static_assert(gtfo::_tt::is_same<
                  std::iterator_traits<const_reverse_iterator>::difference_type,
                  ptrdiff_t
              >::value, "");
static_assert(gtfo::_tt::is_same<
                  std::iterator_traits<const_reverse_iterator>::value_type,
                  gtfo::iterator_range<gtfo::reverse_iterator<const int *>>
              >::value, "");
static_assert(gtfo::_tt::is_same<
                  std::iterator_traits<const_reverse_iterator>::iterator_category,
                  std::random_access_iterator_tag
              >::value, "");

static_assert(gtfo::_tt::is_same<
                  std::iterator_traits<iterator>::value_type,
                  gtfo::iterator_range<int *>
              >::value, "");
static_assert(gtfo::_tt::is_same<
                  std::iterator_traits<iterator>::iterator_category,
                  std::random_access_iterator_tag
              >::value, "");


static void run_iterators_test()
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    iterator it(arr + 3, 3);
    GTFO_TEST_ASSERT_EQ(*it,     std::vector<int>({ 4, 5, 6 }));
    GTFO_TEST_ASSERT_EQ(it[1],   std::vector<int>({ 7, 8, 9 }));

    if (it == it) {}
    if (it != it) {}
    if (it <  it) {}
    if (it >  it) {}
    if (it <= it) {}
    if (it >= it) {}

    const_iterator cit = it;
    GTFO_TEST_ASSERT_EQ(*cit,    std::vector<int>({ 4, 5, 6 }));
    GTFO_TEST_ASSERT_EQ(cit[1],  std::vector<int>({ 7, 8, 9 }));

    if (it == cit) {}
    if (it != cit) {}
    if (it <  cit) {}
    if (it >  cit) {}
    if (it <= cit) {}
    if (it >= cit) {}

    if (cit == it) {}
    if (cit != it) {}
    if (cit <  it) {}
    if (cit >  it) {}
    if (cit <= it) {}
    if (cit >= it) {}

    if (cit == cit) {}
    if (cit != cit) {}
    if (cit <  cit) {}
    if (cit >  cit) {}
    if (cit <= cit) {}
    if (cit >= cit) {}

    reverse_iterator rit { it };
    GTFO_TEST_ASSERT_EQ(*rit,    std::vector<int>({ 3, 2, 1 }));
    GTFO_TEST_ASSERT_EQ(rit[-1], std::vector<int>({ 6, 5, 4 }));

    rit -= 1;

    if (rit == rit) {}
    if (rit != rit) {}
    if (rit <  rit) {}
    if (rit >  rit) {}
    if (rit <= rit) {}
    if (rit >= rit) {}

    iterator rit_base = rit.base();
    GTFO_TEST_ASSERT_EQ(*rit_base,    std::vector<int>({ 7, 8, 9 }));
    GTFO_TEST_ASSERT_EQ(rit_base[-1], std::vector<int>({ 4, 5, 6 }));

    rit_base = (rit + 1).base();
    GTFO_TEST_ASSERT_EQ(*rit_base, std::vector<int>({ 4, 5, 6 }));

    const_reverse_iterator crit = rit;
    GTFO_TEST_ASSERT_EQ(*crit,    std::vector<int>({ 6, 5, 4 }));
    GTFO_TEST_ASSERT_EQ(crit[-1], std::vector<int>({ 9, 8, 7 }));

    if (rit == crit) {}
    if (rit != crit) {}
    if (rit <  crit) {}
    if (rit >  crit) {}
    if (rit <= crit) {}
    if (rit >= crit) {}

    if (crit == rit) {}
    if (crit != rit) {}
    if (crit <  rit) {}
    if (crit >  rit) {}
    if (crit <= rit) {}
    if (crit >= rit) {}

    if (crit == crit) {}
    if (crit != crit) {}
    if (crit <  crit) {}
    if (crit >  crit) {}
    if (crit <= crit) {}
    if (crit >= crit) {}
}

GTFO_TEST_FUN_BEGIN
    ::test_reverse_iterators();
    ::run_iterators_test();
GTFO_TEST_FUN_END