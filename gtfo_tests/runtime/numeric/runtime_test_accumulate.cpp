#include "gtfo/numeric/accumulate.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"
#include <vector>
#include <functional>

using namespace gtfo::runtime_test_helpers;

namespace
{
    class Foo
    {
    private:
        int _data;
    public:
        explicit Foo(int i) : _data(i) { }
        int data() const { return _data; }
    };

    class Boo
    {
    private:
        int _data;
    public:
        explicit Boo(int i) : _data(i) { }
        int data() const { return _data; }
    };

    struct FooPlusBoo
    {
        Foo operator()(const Foo & lhs, const Boo & rhs) const
        {
            return Foo( lhs.data() + rhs.data() );
        }
    };

    inline Foo foo_plus_boo(const Foo & lhs, const Boo & rhs)
    {
        return Foo( lhs.data() + rhs.data() );
    }

    inline Foo operator + (const Foo & lhs, const Boo & rhs)
    {
        return Foo( lhs.data() + rhs.data() );
    }

    inline std::vector<int> make_12345()
    {
        return make_vec5i(1, 2, 3, 4, 5);
    }
}

GTFO_TEST_FUN_BEGIN
    const std::vector<int> vi = make_12345();

    // two iterators
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(vi.begin() + 1, vi.end() - 1),                             9)

    // two iterators + value
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(vi.begin() + 1, vi.end() - 1, 10000),                      10009)

    // two iterators + value + binary operation
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(vi.begin() + 1, vi.end() - 1, -10000, std::plus<int>()),   -9991)

    // range (lvalue or rvalue)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(vi),                                                       15)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(make_12345()),                                             15)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(rev(vi)),                                                  15)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(rev(make_12345())),                                        15)

    // range + value
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(vi,                -100),                                  -85)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(make_12345(),      -100),                                  -85)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(rev(vi),           -100),                                  -85)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(rev(make_12345()), -100),                                  -85)

    // range + value + binary operation
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(vi,                100, std::plus<int>()),                 115)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(make_12345(),      100, std::plus<int>()),                 115)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(rev(vi),           100, std::plus<int>()),                 115)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(rev(make_12345()), 100, std::plus<int>()),                 115)

    const float c_array[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
    GTFO_TEST_ASSERT_NEAR_E3(gtfo::accumulate(c_array, c_array + 2),                                3.f)
    GTFO_TEST_ASSERT_NEAR_E3(gtfo::accumulate(c_array, c_array + 2, 5.f),                           8.f)
    GTFO_TEST_ASSERT_NEAR_E3(gtfo::accumulate(c_array, c_array + 2, 5.f, std::plus<float>()),       8.f)
    GTFO_TEST_ASSERT_NEAR_E3(gtfo::accumulate(c_array),                                             15.f)
    GTFO_TEST_ASSERT_NEAR_E3(gtfo::accumulate(c_array, 5.f),                                        20.f)
    GTFO_TEST_ASSERT_NEAR_E3(gtfo::accumulate(c_array, 5.f, std::multiplies<float>()),              600.f)

    const Boo boo_arr[] = { Boo(1), Boo(9) };
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(boo_arr, Foo(7), FooPlusBoo()).data(),                     17)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(boo_arr, Foo(9), foo_plus_boo).data(),                     19)
    GTFO_TEST_ASSERT_EQ(gtfo::accumulate(boo_arr, Foo(25)).data(),                                  35)
GTFO_TEST_FUN_END
