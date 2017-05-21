#include "gtfo/_impl/type_traits/is_iterator.hpp" // DEPRECATED
#include "gtfo/_impl/type_traits/is_random_access_iterator.hpp"
#include <iterator>
#include <list>
#include <deque>
#include "gtfo/container/matrix.hpp"

#define GTFO_IS_ITERATOR(t)                       ::gtfo::_tt::is_iterator<t>::value // DEPRECATED
#define GTFO_IS_OUTPUT_ITERATOR(t)                (::gtfo::_tt::is_output_iterator<t>::value)
#define GTFO_IS_INPUT_ITERATOR(t)                 (::gtfo::_tt::is_input_iterator<t>::value)

#define GTFO_IS_FORWARD_ITERATOR(t)               (::gtfo::_tt::is_forward_iterator<t>::value)
#define GTFO_IS_MUTABLE_FORWARD_ITERATOR(t)       (::gtfo::_tt::is_mutable_forward_iterator<t>::value)
#define GTFO_IS_CONST_FORWARD_ITERATOR(t)         (::gtfo::_tt::is_forward_iterator<t>::value && !::gtfo::_tt::is_mutable_forward_iterator<t>::value)

#define GTFO_IS_BIDIRECTIONAL_ITERATOR(t)         (::gtfo::_tt::is_bidirectional_iterator<t>::value)
#define GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(t) (::gtfo::_tt::is_mutable_bidirectional_iterator<t>::value)
#define GTFO_IS_CONST_BIDIRECTIONAL_ITERATOR(t)   (::gtfo::_tt::is_bidirectional_iterator<t>::value && !::gtfo::_tt::is_mutable_bidirectional_iterator<t>::value)

#define GTFO_IS_RANDOM_ACCESS_ITERATOR(t)         (::gtfo::_tt::is_random_access_iterator<t>::value)
#define GTFO_IS_MUTABLE_RANDOM_ACCESS_ITERATOR(t) (::gtfo::_tt::is_mutable_random_access_iterator<t>::value)
#define GTFO_IS_CONST_RANDOM_ACCESS_ITERATOR(t)   (::gtfo::_tt::is_random_access_iterator<t>::value && !::gtfo::_tt::is_mutable_random_access_iterator<t>::value)

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct A // not an iterator: operator*() returns void
    {
        void operator++() { }
        void operator*() const { }
        friend bool operator==(const A &, const A &) UNUSED { return true; }
        friend bool operator!=(const A &, const A &) UNUSED { return true; }
    };

    struct B // enough for this library to believe it's a forward iterator
    {
        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const B &, const B &) UNUSED { return true; }
        friend bool operator!=(const B &, const B &) UNUSED { return true; }
    };

    struct B0 // input and output iterator, but not a forward iterator because it is not default constructible
    {
        B0() = delete;
        explicit B0(int) { }

        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const B0 &, const B0 &) UNUSED { return true; }
        friend bool operator!=(const B0 &, const B0 &) UNUSED { return true; }
    };

    struct B1 // input iterator, but not an output iterator because we cannot write into const int &
    {
        void operator++() { }
        const int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const B1 &, const B1 &) UNUSED { return true; }
        friend bool operator!=(const B1 &, const B1 &) UNUSED { return true; }
    };

    struct B2 // input iterator, but not an output iterator because we cannot write into int rvalue
    {
        void operator++() { }
        int operator*() const { return 42; }
        friend bool operator==(const B2 &, const B2 &) UNUSED { return true; }
        friend bool operator!=(const B2 &, const B2 &) UNUSED { return true; }
    };

    struct B3 // output iterator, because it returns an object which can be assigned to
    {
        struct MayBeAProxy { };

        void operator++() { }
        MayBeAProxy operator*() const { return MayBeAProxy(); }
        friend bool operator==(const B3 &, const B3 &) UNUSED { return true; }
        friend bool operator!=(const B3 &, const B3 &) UNUSED { return true; }
    };

    struct B4 // even though it returns a const object, it is still considered an output iterator
    {
        struct StillMayBeAProxy { };

        void operator++() { }
        const StillMayBeAProxy operator*() const { return StillMayBeAProxy(); }
        friend bool operator==(const B4 &, const B4 &) UNUSED { return true; }
        friend bool operator!=(const B4 &, const B4 &) UNUSED { return true; }
    };

    struct C // not an iterator: operator++() is private
    {
    private:
        void operator++() { }
    public:
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const C &, const C &) UNUSED { return true; }
        friend bool operator!=(const C &, const C &) UNUSED { return true; }
    };

    struct D // not an iterator: no operator++() provided
    {
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const D &, const D &) UNUSED { return true; }
        friend bool operator!=(const D &, const D &) UNUSED { return true; }
    };

    struct E // not an iterator: no operator*() provided
    {
        void operator++() { }
        friend bool operator==(const E &, const E &) UNUSED { return true; }
        friend bool operator!=(const E &, const E &) UNUSED { return true; }
    };

    struct F // output iterator, but not an input iterator: no operator==() provided
    {
        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator!=(const F &, const F &) UNUSED { return true; }
    };

    struct G // output iterator, but not an input iterator: no operator!=() provided
    {
        void operator++() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const G &, const G &) UNUSED { return true; }
    };

    struct H // enough for this library to believe it's a bidirectional iterator
    {
        void operator++() { }
        void operator--() { }
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const H &, const H &) UNUSED { return true; }
        friend bool operator!=(const H &, const H &) UNUSED { return true; }
    };

    struct I // forward iterator, but not a bidirectional iterator because operator -- is private
    {
    public:
        void operator++() { }
    private:
        void operator--() { }
    public:
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const I &, const I &) UNUSED { return true; }
        friend bool operator!=(const I &, const I &) UNUSED { return true; }
    };

    struct J // forward iterator, but not a bidirectional iterator because operator -- is deleted
    {
        void operator++() { }
        void operator--() = delete;
        int & operator*() const { static int i = 42; return i; }
        friend bool operator==(const J &, const J &) UNUSED { return true; }
        friend bool operator!=(const J &, const J &) UNUSED { return true; }
    };
}

static_assert(!GTFO_IS_ITERATOR              (A), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (A), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (A), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (A), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(A), "");

static_assert( GTFO_IS_ITERATOR              (B), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (B), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (B), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (B), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(B), "");

static_assert( GTFO_IS_ITERATOR              (B0), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (B0), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (B0), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (B0), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(B0), "");

static_assert( GTFO_IS_ITERATOR              (B1), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (B1), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (B1), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (B1), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(B1), "");

static_assert( GTFO_IS_ITERATOR              (B2), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (B2), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (B2), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (B2), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(B2), "");

static_assert( GTFO_IS_ITERATOR              (B3), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (B3), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (B3), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (B3), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(B3), "");

static_assert( GTFO_IS_ITERATOR              (B4), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (B4), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (B4), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (B4), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(B4), "");

static_assert(!GTFO_IS_ITERATOR              (C), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (C), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (C), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (C), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(C), "");

static_assert(!GTFO_IS_ITERATOR              (D), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (D), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (D), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (D), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(D), "");

static_assert(!GTFO_IS_ITERATOR              (E), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (E), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (E), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (E), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(E), "");

static_assert(!GTFO_IS_ITERATOR              (F), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (F), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (F), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (F), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(F), "");

static_assert(!GTFO_IS_ITERATOR              (G), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (G), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (G), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (G), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(G), "");

static_assert( GTFO_IS_ITERATOR              (H), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (H), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (H), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (H), "");
static_assert( GTFO_IS_BIDIRECTIONAL_ITERATOR(H), "");
static_assert(!GTFO_IS_RANDOM_ACCESS_ITERATOR(H), "");

static_assert( GTFO_IS_ITERATOR              (I), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (I), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (I), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (I), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(I), "");

static_assert( GTFO_IS_ITERATOR              (J), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (J), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (J), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (J), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(J), "");

static_assert(!GTFO_IS_ITERATOR              (void), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (void), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (void), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (void), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(void), "");

static_assert(!GTFO_IS_ITERATOR              (void *), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (void *), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (void *), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (void *), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(void *), "");

static_assert(!GTFO_IS_ITERATOR              (int), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (int), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (int), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (int), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(int), "");

static_assert( GTFO_IS_ITERATOR              (int *), "");
static_assert( GTFO_IS_OUTPUT_ITERATOR       (int *), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (int *), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (int *), "");
static_assert( GTFO_IS_BIDIRECTIONAL_ITERATOR(int *), "");
static_assert( GTFO_IS_RANDOM_ACCESS_ITERATOR(int *), "");

static_assert( GTFO_IS_ITERATOR              (const int *), "");
static_assert(!GTFO_IS_OUTPUT_ITERATOR       (const int *), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (const int *), "");
static_assert( GTFO_IS_FORWARD_ITERATOR      (const int *), "");
static_assert( GTFO_IS_BIDIRECTIONAL_ITERATOR(const int *), "");
static_assert( GTFO_IS_RANDOM_ACCESS_ITERATOR(const int *), "");

static_assert(!GTFO_IS_OUTPUT_ITERATOR       (std::istream_iterator<char>), "");
static_assert( GTFO_IS_INPUT_ITERATOR        (std::istream_iterator<char>), "");

static_assert( GTFO_IS_OUTPUT_ITERATOR       (std::ostream_iterator<char>), "");
static_assert(!GTFO_IS_INPUT_ITERATOR        (std::ostream_iterator<char>), "");
static_assert(!GTFO_IS_FORWARD_ITERATOR      (std::ostream_iterator<char>), "");
static_assert(!GTFO_IS_BIDIRECTIONAL_ITERATOR(std::ostream_iterator<char>), "");

static_assert( GTFO_IS_OUTPUT_ITERATOR               (std::list<int>::iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::list<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_FORWARD_ITERATOR      (std::list<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(std::list<int>::iterator), "");
static_assert(!GTFO_IS_RANDOM_ACCESS_ITERATOR        (std::list<int>::iterator), "");

static_assert(!GTFO_IS_OUTPUT_ITERATOR               (std::list<int>::const_iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::list<int>::const_iterator), "");
static_assert( GTFO_IS_CONST_FORWARD_ITERATOR        (std::list<int>::const_iterator), "");
static_assert( GTFO_IS_CONST_BIDIRECTIONAL_ITERATOR  (std::list<int>::const_iterator), "");
static_assert(!GTFO_IS_RANDOM_ACCESS_ITERATOR        (std::list<int>::const_iterator), "");

static_assert( GTFO_IS_OUTPUT_ITERATOR               (std::list<int>::reverse_iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::list<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_FORWARD_ITERATOR      (std::list<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(std::list<int>::reverse_iterator), "");
static_assert(!GTFO_IS_RANDOM_ACCESS_ITERATOR        (std::list<int>::reverse_iterator), "");

static_assert(!GTFO_IS_OUTPUT_ITERATOR               (std::list<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::list<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_CONST_FORWARD_ITERATOR        (std::list<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_CONST_BIDIRECTIONAL_ITERATOR  (std::list<int>::const_reverse_iterator), "");
static_assert(!GTFO_IS_RANDOM_ACCESS_ITERATOR        (std::list<int>::const_reverse_iterator), "");

static_assert( GTFO_IS_OUTPUT_ITERATOR               (std::deque<int>::iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::deque<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_FORWARD_ITERATOR      (std::deque<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(std::deque<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_RANDOM_ACCESS_ITERATOR(std::deque<int>::iterator), "");

static_assert(!GTFO_IS_OUTPUT_ITERATOR               (std::deque<int>::const_iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::deque<int>::const_iterator), "");
static_assert( GTFO_IS_CONST_FORWARD_ITERATOR        (std::deque<int>::const_iterator), "");
static_assert( GTFO_IS_CONST_BIDIRECTIONAL_ITERATOR  (std::deque<int>::const_iterator), "");
static_assert( GTFO_IS_CONST_RANDOM_ACCESS_ITERATOR  (std::deque<int>::const_iterator), "");

static_assert( GTFO_IS_OUTPUT_ITERATOR               (std::deque<int>::reverse_iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::deque<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_FORWARD_ITERATOR      (std::deque<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(std::deque<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_RANDOM_ACCESS_ITERATOR(std::deque<int>::reverse_iterator), "");

static_assert(!GTFO_IS_OUTPUT_ITERATOR               (std::deque<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_INPUT_ITERATOR                (std::deque<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_CONST_FORWARD_ITERATOR        (std::deque<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_CONST_BIDIRECTIONAL_ITERATOR  (std::deque<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_CONST_RANDOM_ACCESS_ITERATOR  (std::deque<int>::const_reverse_iterator), "");

static_assert( GTFO_IS_MUTABLE_FORWARD_ITERATOR      (gtfo::matrix<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(gtfo::matrix<int>::iterator), "");
static_assert( GTFO_IS_MUTABLE_RANDOM_ACCESS_ITERATOR(gtfo::matrix<int>::iterator), "");
// In current implementation, the library can't tell that const_iterators of our matrix are indeed const,
// because their dereference operator returns proxy objects.
static_assert( GTFO_IS_FORWARD_ITERATOR              (gtfo::matrix<int>::const_iterator), "");
static_assert( GTFO_IS_BIDIRECTIONAL_ITERATOR        (gtfo::matrix<int>::const_iterator), "");
static_assert( GTFO_IS_RANDOM_ACCESS_ITERATOR        (gtfo::matrix<int>::const_iterator), "");

static_assert( GTFO_IS_MUTABLE_FORWARD_ITERATOR      (gtfo::matrix<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_BIDIRECTIONAL_ITERATOR(gtfo::matrix<int>::reverse_iterator), "");
static_assert( GTFO_IS_MUTABLE_RANDOM_ACCESS_ITERATOR(gtfo::matrix<int>::reverse_iterator), "");

static_assert( GTFO_IS_FORWARD_ITERATOR              (gtfo::matrix<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_BIDIRECTIONAL_ITERATOR        (gtfo::matrix<int>::const_reverse_iterator), "");
static_assert( GTFO_IS_RANDOM_ACCESS_ITERATOR        (gtfo::matrix<int>::const_reverse_iterator), "");
