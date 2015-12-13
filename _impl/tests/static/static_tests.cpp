#include "_impl/type_traits.hpp"
#include <vector>

#define s_assert(x) static_assert(x, #x)

using namespace gtfo::_tt;

struct test_no_iterators
{
    int   begin() { return 0; }
    int   end()   { return 0; }
};

using helpers::has_iterator_returning_begin;
using helpers::has_iterator_returning_end;

static_assert(has_begin                   <test_no_iterators>::value == true,  "");
static_assert(has_end                     <test_no_iterators>::value == true,  "");
static_assert(has_iterator_returning_begin<test_no_iterators>::value == false, "");
static_assert(has_iterator_returning_end  <test_no_iterators>::value == false, "");

struct test_iterator_begin_only
{
    int * begin() { return nullptr; }
    int   end()   { return 0;       }
};
static_assert(has_begin                   <test_iterator_begin_only>::value == true,  "");
static_assert(has_end                     <test_iterator_begin_only>::value == true,  "");
static_assert(has_iterator_returning_begin<test_iterator_begin_only>::value == true,  "");
static_assert(has_iterator_returning_end  <test_iterator_begin_only>::value == false, "");

struct test_iterator_end_only
{
    int   begin() { return 0;       }
    int * end()   { return nullptr; }
};
static_assert(has_begin                   <test_iterator_end_only>::value == true,  "");
static_assert(has_end                     <test_iterator_end_only>::value == true,  "");
static_assert(has_iterator_returning_begin<test_iterator_end_only>::value == false, "");
static_assert(has_iterator_returning_end  <test_iterator_end_only>::value == true,  "");

struct test_iterator_both
{
    int * begin() { return nullptr; }
    int * end()   { return nullptr; }
};
static_assert(has_begin                   <test_iterator_both>::value == true, "");
static_assert(has_end                     <test_iterator_both>::value == true, "");
static_assert(has_iterator_returning_begin<test_iterator_both>::value == true, "");
static_assert(has_iterator_returning_end  <test_iterator_both>::value == true, "");

static_assert(is_container< ::std::vector<int>       >::value == true,  "");
static_assert(is_container< test_no_iterators        >::value == false, "");
static_assert(is_container< test_iterator_begin_only >::value == false, "");
static_assert(is_container< test_iterator_end_only   >::value == false, "");
static_assert(is_container< test_iterator_both       >::value == true,  "");

static_assert(is_dereferenceable< int   >::value == false, "");
static_assert(is_dereferenceable< int * >::value == true,  "");

static_assert(is_iterator< std::vector<int>::iterator               >::value == true,  "");
static_assert(is_iterator< std::vector<int>::const_reverse_iterator >::value == true,  "");
static_assert(is_iterator< int                                      >::value == false, "");
static_assert(is_iterator< int *                                    >::value == true,  "");
static_assert(is_iterator< void                                     >::value == false, "");
static_assert(is_iterator< void *                                   >::value == false, "");

static_assert(is_same
              <
                  typename value_of_dereferenced<std::vector<int>::iterator>::type,
                  int
              >::value, "");
static_assert(is_same
              <
                  typename value_of_dereferenced<std::vector<int>::const_iterator>::type,
                  int
              >::value, "");
static_assert(is_same
              <
                  typename result_of_dereferencing<std::vector<int>::iterator>::type,
                  int &
              >::value, "");
static_assert(is_same
              <
                  typename result_of_dereferencing<std::vector<int>::const_iterator>::type,
                  const int &
              >::value, "");

static_assert(is_assignable<long &, long>::value, "");
static_assert(!is_assignable<const long &, long>::value, "");
static_assert(is_assignable<std::ostream_iterator<int> &, int>::value, "");

static_assert(is_dereferenceable< std::ostream_iterator<int> >::value, "");

static_assert(is_same
              <
                  typename value_from_container<std::vector<int>>::type,
                  int
              >::value, "");
static_assert(::std::is_assignable
              <
                  decltype( * declval< std::ostream_iterator<int> &>() ),
                  int
              >::value, "");
static_assert(is_assignable
              <
                  decltype( * declval< std::ostream_iterator<int> &>() ),
                  int
              >::value, "is_assignable");

struct NotComparableEq
{
    friend void operator == (const NotComparableEq &, const NotComparableEq &);
    friend bool operator != (const NotComparableEq &, const NotComparableEq &);
};
struct NotComparableNEq
{
    friend bool operator == (const NotComparableNEq &, const NotComparableNEq &);
    friend void operator != (const NotComparableNEq &, const NotComparableNEq &);
};
struct NotComparableAtAll
{
    friend void operator == (const NotComparableAtAll &, const NotComparableAtAll &);
    friend void operator != (const NotComparableAtAll &, const NotComparableAtAll &);
};
struct Comparable
{
    friend bool operator == (const Comparable &, const Comparable &);
    friend bool operator != (const Comparable &, const Comparable &);
};

template<typename T, typename U>
struct can_invoke_both_equality_comparisons
{
    static const bool value = helpers::can_invoke_comparison_eq<T, U>::value &&
                              helpers::can_invoke_comparison_n_eq<T, U>::value;
};

static_assert(
        can_invoke_both_equality_comparisons<int, int>::value,
        ""
        );
static_assert(
        can_invoke_both_equality_comparisons<int *, const int *volatile>::value,
        ""
        );
static_assert(
        !can_invoke_both_equality_comparisons<int *, int>::value,
        ""
        );
static_assert(
        can_invoke_both_equality_comparisons<NotComparableEq, NotComparableEq>::value,
        ""
        );
static_assert(
        can_invoke_both_equality_comparisons<NotComparableNEq, NotComparableNEq>::value,
        ""
        );
static_assert(
        can_invoke_both_equality_comparisons<NotComparableAtAll, NotComparableAtAll>::value,
        ""
        );
static_assert(
        can_invoke_both_equality_comparisons<Comparable, Comparable>::value,
        ""
        );

struct NotBoolean { };
struct SortaBoolean { operator bool(); };
struct ALittleBitBoolean { operator double(); };
struct TwoConversionBoolean { operator SortaBoolean(); };
struct ShyBoolean { private: operator bool(); };

static_assert(
        helpers::can_be_used_in_a_boolean_context<bool>::value,
        ""
        );
static_assert(
        helpers::can_be_used_in_a_boolean_context<int>::value,
        ""
        );
static_assert(
        helpers::can_be_used_in_a_boolean_context<int *>::value,
        ""
        );
static_assert(
        helpers::can_be_used_in_a_boolean_context<std::nullptr_t>::value,
        ""
        );
static_assert(
        !helpers::can_be_used_in_a_boolean_context<NotBoolean>::value,
        ""
        );
static_assert(
        helpers::can_be_used_in_a_boolean_context<SortaBoolean>::value,
        ""
        );
static_assert(
        helpers::can_be_used_in_a_boolean_context<ALittleBitBoolean>::value,
        ""
        );
static_assert(
        !helpers::can_be_used_in_a_boolean_context<TwoConversionBoolean>::value,
        ""
        );

// unfortunately, a bug in MSVC 2012 :( dunno how to make workarounds for it right now
#if !defined(_MSC_VER) || _MSC_VER >= 1900
static_assert(
        !helpers::can_be_used_in_a_boolean_context<ShyBoolean>::value,
        ""
        );
#endif

static_assert(
        !is_equality_comparable<NotComparableEq, NotComparableEq>::value,
        ""
        );
static_assert(
        !is_equality_comparable<NotComparableNEq, NotComparableNEq>::value,
        ""
        );
static_assert(
        !is_equality_comparable<NotComparableAtAll, NotComparableAtAll>::value,
        ""
        );
static_assert(
        is_equality_comparable<Comparable, Comparable>::value,
        ""
        );
static_assert(
        !is_equality_comparable<std::ostream_iterator<int>, std::ostream_iterator<int>>::value,
        ""
        );
static_assert(
        is_equality_comparable<std::vector<int>::iterator, std::vector<int>::const_iterator>::value,
        ""
        );
static_assert(
        !is_equality_comparable<std::vector<int>::iterator, std::vector<int>::reverse_iterator>::value,
        ""
        );

static_assert(
        is_same
        <
            typename common_type_2<float &, int &>::type,
            float
        >::value,
        ""
        );
static_assert(
        is_same
        <
            typename common_type_2<int &, float &>::type,
            float
        >::value,
        ""
        );
static_assert(
        is_same
        <
            typename common_type_2<const float &, volatile int &>::type,
            float
        >::value,
        ""
        );
