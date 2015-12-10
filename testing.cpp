#include "type_traits.hpp"
#include <vector>

#define s_assert(x) static_assert(x, #x)

using namespace gtfo::tt;

struct test_no_iterators
{
    int   begin() { return 0; }
    int   end()   { return 0; }
};
namespace std {
    auto begin(test_no_iterators &arg) -> decltype(arg.begin()) { return arg.begin(); }
    auto end  (test_no_iterators &arg) -> decltype(arg.end())   { return arg.end  (); }
}
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

static_assert(is_dereferenceable< int    >::value == false, "");
static_assert(is_dereferenceable< int *  >::value == true,  "");

static_assert(is_iterator< std::vector<int>::iterator               >::value == true,  "");
static_assert(is_iterator< std::vector<int>::const_reverse_iterator >::value == true,  "");
static_assert(is_iterator< int                                      >::value == false, "");
static_assert(is_iterator< int *                                    >::value == true,  "");
static_assert(is_iterator< void                                     >::value == false, "");
static_assert(is_iterator< void *                                   >::value == false, "");

static_assert(is_same
              <
                  typename dereferencing_result_value<std::vector<int>::iterator>::type,
                  int
              >::value, "");
static_assert(is_same
              <
                  typename dereferencing_result_value<std::vector<int>::const_iterator>::type,
                  int
              >::value, "");
static_assert(is_same
              <
                  typename declared_dereferencing_result<std::vector<int>::iterator>::type,
                  int &
              >::value, "");
static_assert(is_same
              <
                  typename declared_dereferencing_result<std::vector<int>::const_iterator>::type,
                  const int &
              >::value, "");
static_assert(can_assign_container_element_to_dereferenced_output_iterator
              <
                  std::vector<float>,
                  std::vector<long double>::iterator
              >::value, "");

static_assert(is_assignable<long &, long>::value, "");
static_assert(!is_assignable<const long &, long>::value, "");
static_assert(is_assignable<std::ostream_iterator<int> &, int>::value, "");

static_assert(is_dereferenceable< std::ostream_iterator<int> >::value, "");

static_assert(!can_assign_container_element_to_dereferenced_output_iterator
              <
                  std::vector<float>,
                  std::vector<long double>::const_iterator
              >::value, "");

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
static_assert(can_assign_container_element_to_dereferenced_output_iterator
              <
                std::vector<int>,
                std::ostream_iterator<int>
              >::value, "");
