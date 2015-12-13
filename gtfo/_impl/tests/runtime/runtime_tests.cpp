#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <functional>
#include <iterator>

#include "gtfo/numeric.hpp"
#include "gtfo/algorithm.hpp"

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





void print_if_odd(int arg)
{
    if (arg % 2)
        cout << "odd element: " << arg << endl;
}







int main()
{
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    vi.push_back(4);
    vi.push_back(5);
    cout << accumulate(vi.begin() + 1, vi.end() - 1, 10000) << endl;
    cout << accumulate(vi.begin() + 1, vi.end() - 1, -10000, std::plus<int>()) << endl;
    cout << accumulate(vi, -100) << endl;
    cout << accumulate(vi, 100, std::plus<int>()) << endl;
    cout << accumulate(vi, std::plus<int>()) << endl;

    float c_array[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
    cout << accumulate(c_array, 5.0, std::multiplies<double>()) << endl;

    Boo boo_arr[] = { Boo(1), Boo(9) };
    cout << accumulate(boo_arr, Foo(7), PlusFooBoo()).data() << endl;
    cout << accumulate(boo_arr, Foo(25)).data() << endl;

    adjacent_difference(vi.begin(), vi.end(), std::ostream_iterator<int>(cout));
    endl(cout);
    adjacent_difference(vi, std::ostream_iterator<int>(cout));
    endl(cout);
    adjacent_difference(vi.begin(), vi.end(), std::ostream_iterator<int>(cout), std::minus<int>());
    endl(cout);
    adjacent_difference(vi, std::ostream_iterator<int>(cout), std::minus<int>());
    endl(cout);

    for_each(vi.begin(), vi.end(), [](int x){ cout << "lol:" << x; });
    endl(cout);
    for_each(vi, [](int x){ cout << "lol:" << x; });
    endl(cout);

    cout << inner_product(begin(vi), end(vi), begin(c_array),
                          10000L) <<
            endl;
    cout << inner_product(begin(vi), end(vi), begin(c_array),
                          10000L,
                          std::plus<int>(), std::multiplies<int>()) <<
            endl;
    cout << inner_product(begin(vi), end(vi), begin(c_array)) <<
            endl;


    cout << inner_product(vi, begin(c_array), 42) << endl;
    cout << inner_product(vi, begin(c_array), 42, std::plus<int>(), std::multiplies<int>()) << endl;

    cout << gtfo::inner_product(vi.begin(), vi.end(), c_array, 42) << endl;
    cout << gtfo::inner_product(begin(c_array), end(c_array), vi, 42, std::plus<int>(), std::multiplies<int>()) << endl;

    cout << inner_product(c_array, vi, 100) << ' ' << inner_product(vi, c_array, 100) << endl;
}
