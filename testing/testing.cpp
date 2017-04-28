#include "testing/testing.hpp"

namespace detail
{
    std::atomic<unsigned int> _test_success_count(0);
    std::atomic<unsigned int> _test_failures_count(0);
}

int main()
{
    unsigned int success_count = detail::_test_success_count;
    unsigned int failures_count = detail::_test_failures_count;

    std::cout << "successful tests: " << success_count << "; ";
    std::cout << "failed tests: " << failures_count << std::endl;

#ifdef _MSC_VER
    std::cin.get();
#endif

    return (failures_count == 0) ? 0 : -1;
}