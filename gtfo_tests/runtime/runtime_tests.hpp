#ifndef GTFO_FILE_INCLUDED_RUNTIME_TESTS_HPP
#define GTFO_FILE_INCLUDED_RUNTIME_TESTS_HPP

#include "gtfo/_impl/type_traits/is_range.hpp"
#include "gtfo/reversed_range.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

namespace gtfo
{
    namespace detail
    {
        inline
        const char *
        short_file_name(const char * fn) GTFO_NOEXCEPT
        {
            const char * result = fn;
            if (!fn)
                return "<bad file name>";
            while (*fn != '\0') {
                if (*fn == '\\')
                    result = ++fn;
                else
                    ++fn;
            }
            return result;
        }

        inline
        ::std::ostream &
        print_short_file_name_no_extension(const char * fn, ::std::ostream & os)
        {
            fn = short_file_name(fn);
            size_t num = 0;
            while (fn[num] != '\0' && fn[num] != '.') {
                ++num;
            }
            return os.write(fn, num);
        }

        template<typename T>
        inline
        typename _tt::enable_if
        <
            !_tt::is_range<T>::value,
            ::std::ostream &
        >::type
        print(::std::ostream & os, const T & value)
        {
            return os << value;
        }

        template<typename Range>
        inline
        typename _tt::enable_if
        <
            _tt::is_range<Range>::value,
            ::std::ostream &
        >::type
        print(::std::ostream & os, const Range & range)
        {
            auto it = begin(range);
            const auto e = end(range);
            os << '{' << ' ';
            while (it != e) {
                print(os, *it);
                os << ' ';
                ++it;
            }
            return os << '}';
        }

        template<typename Lhs, typename Rhs>
        inline
        typename _tt::enable_if
        <
            !_tt::is_range<Lhs>::value && !_tt::is_range<Rhs>::value,
            bool
        >::type
        is_equal(const Lhs & lhs, const Rhs & rhs)
        {
            return lhs == rhs;
        }

        template<typename Range1, typename Range2>
        inline
        typename _tt::enable_if
        <
            _tt::is_range<Range1>::value && _tt::is_range<Range2>::value,
            bool
        >::type
        is_equal(const Range1 & lhs, const Range2 & rhs)
        {
            auto lit = begin(lhs);
            const auto le = end(lhs);
            auto rit = begin(rhs);
            const auto re = end(rhs);
            while (lit != le && rit != re) {
                if (!is_equal(*lit, *rit))
                    return false;
                ++lit;
                ++rit;
            }
            if (lit == le && rit == re)
                return true;
            return false;
        }

        inline
        bool
        is_near(long double lhs, long double rhs, long double precision) GTFO_NOEXCEPT
        {
            if (!(precision > 0.0l))
                return false;
            if (lhs < rhs)
                return rhs - lhs < precision;
            else if (rhs < lhs)
                return lhs - rhs < precision;
            else
                return true;
        }

        template<typename Range>
        inline
        typename _tt::enable_if
        <
            _tt::is_range<Range>::value,
            bool
        >::type
        is_near(const Range & lhs, const Range & rhs, long double precision)
        {
            auto lit = begin(lhs);
            const auto le = end(lhs);
            auto rit = begin(rhs);
            const auto re = end(rhs);
            while (lit != le && rit != re) {
                if (!is_near(*lit, *rit, precision))
                    return false;
                ++lit;
                ++rit;
            }
            if (lit == le && rit == re)
                return true;
            return false;
        }
    }
}

#define GTFO_IMPL_TEST_ASSERT_FAILURE_COMMON() \
    { \
        ::std::cerr << "file: " << '\"' << ::gtfo::detail::short_file_name(__FILE__) << '\"' << ::std::endl; \
        ::std::cerr << "line: " << __LINE__ << ::std::endl; \
    }

#define GTFO_TEST_ASSERT(x) \
    { \
        auto && _x = (x); \
        if (!(_x)) { \
            ::std::cerr << "runtime test assertion failed!" << ::std::endl; \
            GTFO_IMPL_TEST_ASSERT_FAILURE_COMMON() \
            ::std::cerr << "expression: " << (#x) << ::std::endl; \
            ::std::exit(1); \
        } \
    }

#define GTFO_TEST_ASSERT_EQ(v, e) \
    { \
        auto && _v = (v); \
        auto && _e = (e); \
        if (!::gtfo::detail::is_equal(_v, _e)) { \
            ::std::cerr << "runtime test equality assertion failed!" << ::std::endl; \
            GTFO_IMPL_TEST_ASSERT_FAILURE_COMMON() \
            ::std::cerr << "actual " << (#v) << ": "; \
            ::gtfo::detail::print(::std::cerr, _v) << ::std::endl; \
            ::std::cerr << "expected: " << (#v) << " == "; \
            ::gtfo::detail::print(::std::cerr, _e) << ::std::endl; \
            ::std::exit(1); \
        } \
    }

#define GTFO_TEST_ASSERT_NEAR(v, e, p) \
    { \
        auto && _v = (v); \
        auto && _e = (e); \
        auto && _p = (p); \
        if (!::gtfo::detail::is_near(_v, _e, _p)) { \
            ::std::cerr << "runtime test near-equality assertion failed!" << ::std::endl; \
            GTFO_IMPL_TEST_ASSERT_FAILURE_COMMON() \
            ::std::cerr << "actual " << (#v) << ": "; \
            ::gtfo::detail::print(::std::cerr, _v) << ::std::endl; \
            ::std::cerr << "expected: " << (#v) << " == "; \
            ::gtfo::detail::print(::std::cerr, _e) << ::std::endl; \
            ::std::cerr << "with precision == " << _p << ::std::endl; \
            ::std::exit(1); \
        } \
    }

#define GTFO_TEST_ASSERT_NEAR_E3(v, e) \
    GTFO_TEST_ASSERT_NEAR(v, e, 0.001l)

#define GTFO_IMPL_TEST_FUN_BEGIN_PRINT_MESSAGE() \
    { \
        ::std::cout << "running "; \
        ::gtfo::detail::print_short_file_name_no_extension(__FILE__, ::std::cout) << ::std::endl; \
    }

#define GTFO_TEST_FUN_BEGIN \
    namespace \
    { \
        inline void test() \
        { \
            GTFO_IMPL_TEST_FUN_BEGIN_PRINT_MESSAGE()

#define GTFO_TEST_FUN_END \
        } \
        \
        struct tester \
        { \
            tester() \
            { \
                test(); \
            } \
        } _tester; \
    }

namespace gtfo
{
    namespace runtime_test_helpers
    {
        using gtfo::begin;
        using gtfo::end;
        using gtfo::rbegin;
        using gtfo::rend;
        using gtfo::rev;

        template<typename T>
        inline
        std::vector<T>
        make_vec3(const T & e1, const T & e2, const T & e3)
        {
            std::vector<T> v;
            v.push_back(e1);
            v.push_back(e2);
            v.push_back(e3);
            return v;
        }

        template<typename T>
        inline
        std::vector<T>
        make_vec5(const T & e1, const T & e2, const T & e3, const T & e4, const T & e5)
        {
            std::vector<T> v;
            v.push_back(e1);
            v.push_back(e2);
            v.push_back(e3);
            v.push_back(e4);
            v.push_back(e5);
            return v;
        }

        template<typename T>
        inline
        std::vector<T>
        make_vec10(const T & e1, const T & e2, const T & e3, const T & e4, const T & e5, const T & e6, const T & e7, const T & e8, const T & e9, const T & e10)
        {
            std::vector<T> v;
            v.push_back(e1);
            v.push_back(e2);
            v.push_back(e3);
            v.push_back(e4);
            v.push_back(e5);
            v.push_back(e6);
            v.push_back(e7);
            v.push_back(e8);
            v.push_back(e9);
            v.push_back(e10);
            return v;
        }

        inline
        std::vector<int>
        make_vec3i(int i1, int i2, int i3)
        {
            return make_vec3<int>(i1, i2, i3);
        }

        inline
        std::vector<int>
        make_vec5i(int i1, int i2, int i3, int i4, int i5)
        {
            return make_vec5<int>(i1, i2, i3, i4, i5);
        }

        inline
        std::vector<int>
        make_vec10i(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10)
        {
            return make_vec10<int>(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
        }

        inline
        std::vector<float>
        make_vec5f(float f1, float f2, float f3, float f4, float f5)
        {
            return make_vec5<float>(f1, f2, f3, f4, f5);
        }
    }
}

#endif // GTFO_FILE_INCLUDED_RUNTIME_TESTS_HPP
