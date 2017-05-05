#include "gtfo/algorithm/count_if.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

using namespace gtfo::runtime_test_helpers;

namespace
{
    struct Foo
    {
        int x;
        Foo() : x(42) { }
        explicit Foo(int x) : x(x) { }
    };

    class Bool
    {
    private:
        bool _value;
    public:
        explicit Bool(bool value) : _value(value) { }
        operator bool() const { return _value; }
    };

    class Pred
    {
    private:
        int _x;
    public:
        explicit Pred(int x) : _x(x) { }
        Bool operator()(const Foo & foo) const { return Bool(foo.x == _x); }
    };
}

using gtfo::count_if;

GTFO_TEST_FUN_BEGIN
    Foo arr[10];
    arr[3].x = 10;
    arr[5].x = 10;
    arr[8].x = 10;

    // two iterators + value
    GTFO_TEST_ASSERT_EQ(count_if(arr, arr + 10, Pred(42)), 7)

    // range + value
    GTFO_TEST_ASSERT_EQ(count_if(rev(arr), Pred(10)), 3)
GTFO_TEST_FUN_END
