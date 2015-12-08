#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <functional>

#include "numeric.hpp"

using namespace gtfo;

using std::cout;
using std::endl;

class Foo
{
private:
    int _data;
public:
    explicit Foo(int i) : _data(i) { }
    int data() const noexcept { return _data; }
};

class Boo
{
private:
    int _data;
public:
    explicit Boo(int i) : _data(i) { }
    int data() const noexcept { return _data; }
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

int main()
{
    std::vector<int> vi { 1, 2, 3, 4, 5 };
    cout << accumulate(vi.begin() + 1, vi.end() - 1, 10000) << endl;
    cout << accumulate(vi.begin() + 1, vi.end() - 1, -10000, std::plus<int>()) << endl;
    cout << accumulate(vi, -100) << endl;
    cout << accumulate(vi, 100, std::plus<int>()) << endl;
    cout << accumulate(vi, std::plus<int>()) << endl;

    float c_array[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
    cout << accumulate(c_array, 5.0, std::multiplies<float>()) << endl;

    Boo boo_arr[] = { Boo(1), Boo(9) };
    cout << accumulate(boo_arr, Foo(7), PlusFooBoo()).data() << endl;
    cout << accumulate(boo_arr, Foo(25)).data() << endl;
}

