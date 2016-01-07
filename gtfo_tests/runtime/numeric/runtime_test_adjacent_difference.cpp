#include "gtfo/numeric/adjacent_difference.hpp"
#include "gtfo/reversed_range.hpp"
#include "gtfo_tests/runtime/runtime_tests.hpp"
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>

using namespace gtfo::runtime_test_helpers;

namespace
{
    inline std::vector<int> make_12345()
    {
        return make_vec5<int>(1, 2, 3, 4, 5);
    }

    inline void test_main_cases_raw_array()
    {
        using gtfo::begin;
        using gtfo::end;
        using gtfo::rev;

        const float c_array[] = { 10, 12, 15, 19, 24 };
        std::vector<float> dest(end(c_array) - begin(c_array));

        // two iterators
        gtfo::adjacent_difference(begin(c_array), end(c_array), dest.begin());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10, 2, 3, 4, 5));

        // two iterators + binary operation
        gtfo::adjacent_difference(begin(c_array), end(c_array), dest.begin(), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10.f, 1.2f, 1.25f, 1.266667f, 1.263158f));

        // lvalue range
        gtfo::adjacent_difference(c_array, dest.begin());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10, 2, 3, 4, 5));

        // lvalue range + binary operation
        gtfo::adjacent_difference(c_array, dest.begin(), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10.f, 1.2f, 1.25f, 1.266667f, 1.263158f));

        // prvalue range
        gtfo::adjacent_difference(make_vec5f(10, 12, 15, 19, 24), dest.begin());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10, 2, 3, 4, 5));

        // prvalue range + binary operation
        gtfo::adjacent_difference(make_vec5f(10, 12, 15, 19, 24), dest.begin(), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10.f, 1.2f, 1.25f, 1.266667f, 1.263158f));

        // reversed lvalue range
        gtfo::adjacent_difference(rev(c_array), dest.begin());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24, -5, -4, -3, -2));

        // reversed lvalue range + binary operation
        gtfo::adjacent_difference(rev(c_array), dest.begin(), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24.f, 0.791667f, 0.789473f, 0.8f, 0.833333f));

        // reversed prvalue range
        gtfo::adjacent_difference(rev(make_vec5f(10, 12, 15, 19, 24)), dest.begin());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24, -5, -4, -3, -2));

        // reversed prvalue range + binary operation
        gtfo::adjacent_difference(rev(make_vec5f(10, 12, 15, 19, 24)), dest.begin(), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24.f, 0.791667f, 0.789473f, 0.8f, 0.833333f));
    }

    inline void test_back_inserter()
    {
        using gtfo::begin;
        using gtfo::end;
        using gtfo::rev;

        const std::vector<float> vf = make_vec5f(10, 12, 15, 19, 24);
        std::vector<float> dest;

        // two iterators
        dest.clear();
        gtfo::adjacent_difference(begin(vf), end(vf), std::back_inserter(dest));
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10, 2, 3, 4, 5));

        // two iterators + binary operation
        dest.clear();
        gtfo::adjacent_difference(begin(vf), end(vf), std::back_inserter(dest), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10.f, 1.2f, 1.25f, 1.266667f, 1.263158f));

        // lvalue range
        dest.clear();
        gtfo::adjacent_difference(vf, std::back_inserter(dest));
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10, 2, 3, 4, 5));

        // lvalue range + binary operation
        dest.clear();
        gtfo::adjacent_difference(vf, std::back_inserter(dest), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10.f, 1.2f, 1.25f, 1.266667f, 1.263158f));

        // prvalue range
        dest.clear();
        gtfo::adjacent_difference(make_vec5f(10, 12, 15, 19, 24), std::back_inserter(dest));
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10, 2, 3, 4, 5));

        // prvalue range + binary operation
        dest.clear();
        gtfo::adjacent_difference(make_vec5f(10, 12, 15, 19, 24), std::back_inserter(dest), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(10.f, 1.2f, 1.25f, 1.266667f, 1.263158f));

        // reversed lvalue range
        dest.clear();
        gtfo::adjacent_difference(rev(vf), std::back_inserter(dest));
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24, -5, -4, -3, -2));

        // reversed lvalue range + binary operation
        dest.clear();
        gtfo::adjacent_difference(rev(vf), std::back_inserter(dest), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24.f, 0.791667f, 0.789473f, 0.8f, 0.833333f));

        // reversed prvalue range
        dest.clear();
        gtfo::adjacent_difference(rev(make_vec5f(10, 12, 15, 19, 24)), std::back_inserter(dest));
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24, -5, -4, -3, -2));

        // reversed prvalue range + binary operation
        dest.clear();
        gtfo::adjacent_difference(rev(make_vec5f(10, 12, 15, 19, 24)), std::back_inserter(dest), std::divides<float>());
        GTFO_TEST_ASSERT_NEAR_E3(dest, make_vec5f(24.f, 0.791667f, 0.789473f, 0.8f, 0.833333f));
    }

    inline void test_ostream_iterator()
    {
        typedef std::ostream_iterator<int> oit;
        using std::cout;
        using std::endl;
        using gtfo::rev;

        const std::vector<int> vi = make_12345();

        // two iterators
        cout << '\t';
        gtfo::adjacent_difference(vi.begin(), vi.end(), oit(cout));
        cout << endl;

        // two iterators + binary operation
        cout << '\t';
        gtfo::adjacent_difference(vi.begin(), vi.end(), oit(cout), std::minus<int>());
        cout << endl;

        // lvalue range
        cout << '\t';
        gtfo::adjacent_difference(vi, oit(cout));
        cout << endl;

        // lvalue range + binary operation
        cout << '\t';
        gtfo::adjacent_difference(vi, oit(cout), std::minus<int>());
        cout << endl;

        // prvalue range
        cout << '\t';
        gtfo::adjacent_difference(make_12345(), oit(cout));
        cout << endl;

        // prvalue range + binary operation
        cout << '\t';
        gtfo::adjacent_difference(make_12345(), oit(cout), std::minus<int>());
        cout << endl;

        // reversed lvalue range
        cout << '\t';
        gtfo::adjacent_difference(rev(vi), oit(cout));
        cout << endl;

        // reversed lvalue range + binary operation
        cout << '\t';
        gtfo::adjacent_difference(rev(vi), oit(cout), std::minus<int>());
        cout << endl;

        // reversed prvalue range
        cout << '\t';
        gtfo::adjacent_difference(rev(make_12345()), oit(cout));
        cout << endl;

        // reversed prvalue range + binary operation
        cout << '\t';
        gtfo::adjacent_difference(rev(make_12345()), oit(cout), std::minus<int>());
        cout << endl;
    }
}

GTFO_TEST_FUN_BEGIN
    test_main_cases_raw_array();
    test_back_inserter();
    volatile bool _false = false;
    if (_false)
        test_ostream_iterator(); // test if it compiles; no need to actually spam into stdout
GTFO_TEST_FUN_END
