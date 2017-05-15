#include "gtfo_testing/runtime/runtime_tests.hpp"
#include <cstring>

namespace gtfo
{
    namespace detail
    {
        std::atomic<unsigned int> & _test_success_count()  { static std::atomic<unsigned int> _value(0); return _value; }
        std::atomic<unsigned int> & _test_failures_count() { static std::atomic<unsigned int> _value(0); return _value; }

        bool is_equal(const std::string & lhs, const char * rhs) noexcept
        {
            if (!rhs)
                return false;
            return lhs == rhs;
        }

        bool is_equal(const char * lhs, const std::string & rhs) noexcept
        {
            if (!lhs)
                return false;
            return lhs == rhs;
        }

        bool is_equal(const char * lhs, const char * rhs) noexcept
        {
            if (!lhs && !rhs)
                return true;
            if (!lhs || !rhs)
                return false;
            return ::std::strcmp(lhs, rhs) == 0;
        }
    }
}

int main()
{
    unsigned int success_count = ::gtfo::detail::_test_success_count();
    unsigned int failures_count = ::gtfo::detail::_test_failures_count();

    std::cout << "successful tests: " << success_count << "; ";
    std::cout << "failed tests: " << failures_count << std::endl;

#ifdef _MSC_VER
    std::cin.get();
#endif

    return (failures_count == 0) ? 0 : -1;
}
