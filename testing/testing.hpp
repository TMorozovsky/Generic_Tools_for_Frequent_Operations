#ifndef TESTING_TESTING_HPP_INCLUDED
#define TESTING_TESTING_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
#include <atomic>
#include <cassert>

namespace detail
{
    class test_failure_exception
    {
    };

    inline const char * short_file_name(const char * fn) noexcept
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

    inline std::ostream & print_short_file_name_no_extension(const char * fn, std::ostream & os)
    {
        fn = short_file_name(fn);
        size_t num = 0;
        while (fn[num] != '\0' && fn[num] != '.') {
            ++num;
        }
        return os.write(fn, num);
    }

    extern std::atomic<unsigned int> _test_success_count;
    extern std::atomic<unsigned int> _test_failures_count;
}

#define IMPL_TEST_ASSERT_FAILURE_COMMON() \
    { \
        std::cerr << "file: " << '\"' << detail::short_file_name(__FILE__) << '\"' << '\n'; \
        std::cerr << "line: " << __LINE__ << '\n'; \
    }

#define ASSERT(x) \
    { \
        auto && _x = (x); \
        if (!(_x)) { \
            std::cerr << "runtime test assertion failed!" << '\n'; \
            IMPL_TEST_ASSERT_FAILURE_COMMON() \
            std::cerr << "expression: " << (#x) << '\n'; \
            throw detail::test_failure_exception(); \
        } \
    }

#define ASSERT_EQ(v, e) \
    { \
        auto && _v = (v); \
        auto && _e = (e); \
        if (!(_v == _e)) { \
            std::cerr << "runtime test equality assertion failed!" << '\n'; \
            IMPL_TEST_ASSERT_FAILURE_COMMON() \
            std::cerr << "actual " << (#v) << ": "; \
            std::cerr << _v << '\n'; \
            std::cerr << "expected: " << (#v) << " == "; \
            std::cerr << _e << '\n'; \
            throw detail::test_failure_exception(); \
        } \
    }

#define IMPL_TEST_FUN_BEGIN_PRINT_MESSAGE() \
    { \
        std::cout << "running "; \
        detail::print_short_file_name_no_extension(__FILE__, std::cout) << '\n'; \
    }

#define TEST_FUN_BEGIN \
    namespace \
    { \
        inline void test() \
        { \
            try \
            { \
                IMPL_TEST_FUN_BEGIN_PRINT_MESSAGE()

#define TEST_FUN_END \
                ++detail::_test_success_count; \
            } \
            catch (const detail::test_failure_exception &) \
            { \
                ++detail::_test_failures_count; \
            } \
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

// simple replacement for std::unique_ptr that's used in testing functions
template <typename T>
class simple_unique_ptr
{
private:
    T * _ptr;

public:
    simple_unique_ptr() noexcept : _ptr(nullptr) { }
    explicit simple_unique_ptr(T * owning_raw_pointer) noexcept : _ptr(owning_raw_pointer) { }
    simple_unique_ptr(simple_unique_ptr && other) noexcept : _ptr(other._ptr) { other._ptr = nullptr; }
    simple_unique_ptr(const simple_unique_ptr &) = delete;
    simple_unique_ptr & operator = (simple_unique_ptr && other) noexcept { delete _ptr; _ptr = other._ptr; other._ptr = nullptr; return *this; }
    simple_unique_ptr & operator = (const simple_unique_ptr &) = delete;
    ~simple_unique_ptr() noexcept { delete _ptr; }

    explicit operator bool () const noexcept { return _ptr != nullptr; }
    bool     operator !    () const noexcept { return _ptr == nullptr; }

    T & operator *  () const noexcept { assert(_ptr); return *_ptr; }
    T * operator -> () const noexcept { assert(_ptr); return _ptr; }

    T * get() const noexcept { return _ptr; }

    void reset()                       { delete _ptr; _ptr = nullptr;            }
    void reset(T * owning_raw_pointer) { delete _ptr; _ptr = owning_raw_pointer; }
};

#endif // TESTING_TESTING_HPP_INCLUDED
