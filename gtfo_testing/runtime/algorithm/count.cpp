#include "gtfo/algorithm/count.hpp"
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

    inline Bool operator == (const Foo & lhs, const Foo & rhs)
    {
        return Bool(lhs.x == rhs.x);
    }
}

using gtfo::count;

GTFO_TEST_FUN_BEGIN
    Foo arr[10];
    arr[3].x = 10;
    arr[5].x = 10;
    arr[8].x = 10;

    // two iterators + value
    GTFO_TEST_ASSERT_EQ(count(arr, arr + 10, Foo(42)), 7)

    // range + value
    GTFO_TEST_ASSERT_EQ(count(arr, Foo(42)), 7)
GTFO_TEST_FUN_END
