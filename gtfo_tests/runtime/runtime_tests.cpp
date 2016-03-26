#include <iostream>

int main()
{
#if defined(_MSC_VER) && defined(GTFO_MSVC_RUNTIME_TESTS_CIN_GET)
    std::cin.get();
#endif
}
