#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <functional>
#include <iterator>

#include "gtfo/numeric.hpp"
#include "gtfo/algorithm.hpp"
#include "gtfo/reversed_range.hpp"

using namespace gtfo;

using std::cout;
using std::endl;

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

struct PlusFooBoo
{
    Foo operator()(const Foo & lhs, const Boo & rhs) const
    {
        return Foo( lhs.data() + rhs.data() );
    }
};

Foo operator + (const Foo & lhs, const Boo & rhs)
{
    return Foo( lhs.data() + rhs.data() );
}

static inline std::vector<int> make_12345()
{
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    vi.push_back(4);
    vi.push_back(5);
    return vi;
}

int main()
{
    const std::vector<int> vi = make_12345();
    const float c_array[] = { 1.f, 2.f, 3.f, 4.f, 5.f };

    cout << std::boolalpha;

    {
        cout << gtfo::accumulate(vi.begin() + 1, vi.end() - 1, 10000) << endl;
        cout << gtfo::accumulate(vi.begin() + 1, vi.end() - 1, -10000, std::plus<int>()) << endl;
        cout << gtfo::accumulate(vi, -100) << endl;
        cout << gtfo::accumulate(vi, 100, std::plus<int>()) << endl;

        cout << gtfo::accumulate(c_array, 5.0, std::multiplies<double>()) << endl;

        Boo boo_arr[] = { Boo(1), Boo(9) };
        cout << gtfo::accumulate(boo_arr, Foo(7), PlusFooBoo()).data() << endl;
        cout << gtfo::accumulate(boo_arr, Foo(25)).data() << endl;
    }

    {
        gtfo::adjacent_difference(vi.begin(), vi.end(), std::ostream_iterator<int>(cout));
        endl(cout);
        gtfo::adjacent_difference(vi, std::ostream_iterator<int>(cout));
        endl(cout);
        gtfo::adjacent_difference(vi.begin(), vi.end(), std::ostream_iterator<int>(cout), std::minus<int>());
        endl(cout);
        gtfo::adjacent_difference(vi, std::ostream_iterator<int>(cout), std::minus<int>());
        endl(cout);
    }

    {
        cout << gtfo::inner_product(begin(vi), end(vi), begin(c_array),
                                    10000L) <<
                endl;
        cout << gtfo::inner_product(begin(vi), end(vi), begin(c_array),
                                    10000L,
                                    std::plus<int>(), std::multiplies<int>()) <<
                endl;
        cout << gtfo::inner_product(begin(vi), end(vi), begin(c_array)) <<
                endl;


        cout << gtfo::inner_product(vi, begin(c_array), 42) << endl;
        cout << gtfo::inner_product(vi, begin(c_array), 42, std::plus<int>(), std::multiplies<int>()) << endl;

        cout << gtfo::inner_product(vi.begin(), vi.end(), c_array, 42) << endl;
        cout << gtfo::inner_product(begin(c_array), end(c_array), vi, 42, std::plus<int>(), std::multiplies<int>()) << endl;

        cout << gtfo::inner_product(c_array, vi, 100) << ' ' <<
                gtfo::inner_product(vi, c_array, 100) << endl;

        cout << gtfo::inner_product(c_array, vi, 100, std::plus<int>(), std::multiplies<int>()) << ' ' <<
                gtfo::inner_product(vi, c_array, 100, std::plus<int>(), std::multiplies<int>()) << endl;

        cout << gtfo::inner_product(c_array, vi.begin()) << ' ' <<
                gtfo::inner_product(vi, begin(c_array)) << endl;

        cout << gtfo::inner_product(begin(c_array), end(c_array), vi) << ' ' <<
                gtfo::inner_product(vi.begin(), vi.end(), c_array) << endl;

        cout << gtfo::inner_product(c_array, vi) << ' ' <<
                gtfo::inner_product(vi, c_array) << endl;
    }

    {
        std::vector<int> ps;
        ps.push_back(1);
        ps.push_back(2);
        ps.push_back(3);
        ps.push_back(4);
        ps.push_back(5);
        gtfo::partial_sum(ps.begin(), ps.end(), ps.begin());
        for (int x : ps) { cout << x << ' '; } cout << endl;
        gtfo::partial_sum(ps, ps.begin());
        for (int x : ps) { cout << x << ' '; } cout << endl;
        gtfo::partial_sum(ps.begin(), ps.end(), ps.begin(), std::plus<int>());
        for (int x : ps) { cout << x << ' '; } cout << endl;
        gtfo::partial_sum(ps, ps.begin(), std::plus<int>());
        for (int x : ps) { cout << x << ' '; } cout << endl;
        gtfo::partial_sum(ps, std::ostream_iterator<int>(cout, " "), std::multiplies<int>()); endl(cout);
    }

    {
        std::vector<int> xs(10);
        std::vector<int> backup = xs;
        gtfo::iota(xs.begin(), xs.end(), 0);
        for (int x : xs) { cout << x << ' '; } cout << endl;
        xs = backup;
        gtfo::iota(xs.begin(), xs.end());
        for (int x : xs) { cout << x << ' '; } cout << endl;
        xs = backup;
        gtfo::iota(xs, 1);
        for (int x : xs) { cout << x << ' '; } cout << endl;
        xs = backup;
        gtfo::iota(xs);
        for (int x : xs) { cout << x << ' '; } cout << endl;
        xs = backup;

        float fs[4];
        gtfo::iota(fs);
        for (float f : fs) { cout << f << ' '; } cout << endl;
        gtfo::iota(fs, 3.14f);
        for (float f : fs) { cout << f << ' '; } cout << endl;
    }

    {
        gtfo::for_each(vi.begin(), vi.end(), [](int x){ cout << "test:" << x; });
        endl(cout);
        gtfo::for_each(vi, [](int x){ cout << "test:" << x; });
        endl(cout);
    }

    {
        cout << gtfo::all_of(vi.begin(), vi.end(),  [](int x){ return x > 0; }) << endl;
        cout << gtfo::all_of(vi,                    [](int x){ return x < 0; }) << endl;

        cout << gtfo::any_of(vi.begin(), vi.end(),  [](int x){ return x == 5; }) << endl;
        cout << gtfo::any_of(vi,                    [](int x){ return x == 7; }) << endl;

        cout << gtfo::none_of(vi.begin(), vi.end(), [](int x){ return x == 7; }) << endl;
        cout << gtfo::none_of(vi,                   [](int x){ return x == 5; }) << endl;
    }





    { // testing of reversed ranges
        {
            auto rb = gtfo::rbegin(c_array);
            auto re = gtfo::rend(c_array);

            cout << "reversed C-style array: \t";
            for (auto rit = rb; rit != re; ++rit)
                cout << *rit << ' ';
            cout << endl;
        }
        {
            auto rb = gtfo::rbegin(vi);
            auto re = gtfo::rend(vi);

            cout << "reversed C++ vector: \t";
            for (auto rit = rb; rit != re; ++rit)
                cout << *rit << ' ';
            cout << endl;
        }
        {
            cout << "reversed C-style array: \t";
            for_each(rev(c_array), [](int x){ cout << "test:"<<x << ' '; });
            cout << endl;
        }
        {
            cout << "reversed C++ vector: \t";
            for_each(rev(vi), [](int x){ cout << "test:"<<x << ' '; });
            cout << endl;
        }
        {
            cout << "reversed C++ vector (prvalue): \t";
            for_each(rev(make_12345()), [](int x){ cout << "test:"<<x << ' '; });
            cout << endl;
        }
        {
            cout << "reversed C++ vector (xvalue): \t";
            auto tmp = make_12345();
            for_each(rev(::gtfo::move(tmp)), [](int x){ cout << "test:"<<x << ' '; });
            cout << endl;
        }
    }

#if defined(_MSC_VER) && !defined(GTFO_MSVC_RUNTIME_TESTS_NO_CIN_GET)
    std::cin.get();
#endif
}
