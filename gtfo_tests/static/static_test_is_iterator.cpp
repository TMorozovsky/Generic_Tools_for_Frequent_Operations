#include "gtfo/_impl/type_traits/is_iterator.hpp"
#define GTFO_IS_ITERATOR(t) ::gtfo::_tt::is_iterator<t>::value

namespace
{
    struct A // not an iterator: operator*() returns void
    {
        void operator++() { }
        void operator*() const { }
        friend bool operator==(const A &, const A &) { return true; }
        friend bool operator!=(const A &, const A &) { return true; }
    };

    struct B // enough for this library to believe it's an iterator
    {
        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const B &, const B &) { return true; }
        friend bool operator!=(const B &, const B &) { return true; }
    };

    struct C // not an iterator: operator++() is private
    {
    private:
        void operator++() { }
    public:
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const C &, const C &) { return true; }
        friend bool operator!=(const C &, const C &) { return true; }
    };

    struct D // not an iterator: no operator++() provided
    {
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const D &, const D &) { return true; }
        friend bool operator!=(const D &, const D &) { return true; }
    };

    struct E // not an iterator: no operator*() provided
    {
        void operator++() { }
        friend bool operator==(const E &, const E &) { return true; }
        friend bool operator!=(const E &, const E &) { return true; }
    };

    struct F // not an iterator: no operator==() provided
    {
        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator!=(const F &, const F &) { return true; }
    };

    struct G // not an iterator: no operator!=() provided
    {
        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const G &, const G &) { return true; }
    };
}

static_assert(!GTFO_IS_ITERATOR(A), "");
static_assert( GTFO_IS_ITERATOR(B), "");
#ifndef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC // a bug in VC 2012
static_assert(!GTFO_IS_ITERATOR(C), "");
#endif
static_assert(!GTFO_IS_ITERATOR(D), "");
static_assert(!GTFO_IS_ITERATOR(E), "");
static_assert(!GTFO_IS_ITERATOR(F), "");
static_assert(!GTFO_IS_ITERATOR(G), "");

static_assert(!GTFO_IS_ITERATOR(void), "");
static_assert(!GTFO_IS_ITERATOR(void *), "");
static_assert(!GTFO_IS_ITERATOR(int), "");
static_assert( GTFO_IS_ITERATOR(int *), "");
