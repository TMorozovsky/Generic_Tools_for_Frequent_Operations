#include "gtfo/functional.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"
#include <string>

GTFO_TEST_FUN_BEGIN
    {
        GTFO_TEST_ASSERT(!gtfo::equal_to()(3, 4));
        GTFO_TEST_ASSERT(gtfo::equal_to()(3, 3));
        const gtfo::equal_to equal_to_obj;
        GTFO_TEST_ASSERT(equal_to_obj(std::string("Hello world"), "Hello world"));
        gtfo::equal_to::is_transparent * p = nullptr; (void)p;
    }
    {
        GTFO_TEST_ASSERT(gtfo::not_equal_to()(3, 4));
        GTFO_TEST_ASSERT(!gtfo::not_equal_to()(3, 3));
        const gtfo::not_equal_to not_equal_to_obj;
        GTFO_TEST_ASSERT(not_equal_to_obj(std::string("Hello world"), "Hello world!!!"));
        gtfo::not_equal_to::is_transparent * p = nullptr; (void)p;
    }
    {
        GTFO_TEST_ASSERT(!gtfo::greater()(3, 4));
        GTFO_TEST_ASSERT(!gtfo::greater()(3, 3));
        const gtfo::greater greater_obj;
        GTFO_TEST_ASSERT(greater_obj(3, 2));
        gtfo::greater::is_transparent * p = nullptr; (void)p;
    }
    {
        GTFO_TEST_ASSERT(gtfo::less()(3, 4));
        GTFO_TEST_ASSERT(!gtfo::less()(3, 3));
        const gtfo::less less_obj;
        GTFO_TEST_ASSERT(!less_obj(3, 2));
        gtfo::less::is_transparent * p = nullptr; (void)p;
    }
    {
        GTFO_TEST_ASSERT(!gtfo::greater_equal()(3, 4));
        GTFO_TEST_ASSERT(gtfo::greater_equal()(3, 3));
        const gtfo::greater_equal greater_equal_obj;
        GTFO_TEST_ASSERT(greater_equal_obj(3, 2));
        gtfo::greater_equal::is_transparent * p = nullptr; (void)p;
    }
    {
        GTFO_TEST_ASSERT(gtfo::less_equal()(3, 4));
        GTFO_TEST_ASSERT(gtfo::less_equal()(3, 3));
        const gtfo::less_equal less_equal_obj;
        GTFO_TEST_ASSERT(!less_equal_obj(3, 2));
        gtfo::less_equal::is_transparent * p = nullptr; (void)p;
    }
GTFO_TEST_FUN_END
