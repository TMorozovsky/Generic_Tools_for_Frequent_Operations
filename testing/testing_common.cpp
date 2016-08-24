#ifdef _MSC_VER
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>

namespace {
    // I'm too lazy to call fflush(stdout) after each printf() -__-
    struct stdout_buffering_disabler {
        stdout_buffering_disabler() {
            std::setbuf(stdout, nullptr);
        }
    } stdout_buffering_disabler_instance;
}