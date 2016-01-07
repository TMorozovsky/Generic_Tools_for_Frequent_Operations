#include <iostream>

int main()
{
#if defined(_MSC_VER) && !defined(GTFO_MSVC_RUNTIME_TESTS_NO_CIN_GET)
    std::cin.get();
#endif
}
