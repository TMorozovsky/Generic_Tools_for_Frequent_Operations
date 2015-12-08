#include "type_traits.hpp"
#include <vector>

using namespace gtfo;

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


