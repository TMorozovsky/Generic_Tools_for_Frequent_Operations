#include "gtfo/functional.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"
#include <string>

GTFO_TEST_FUN_BEGIN
    GTFO_TEST_ASSERT_EQ(gtfo::plus()(3, 4), 7);
    GTFO_TEST_ASSERT_NEAR_E6(gtfo::plus()(3, 4.5), 7.5);
    const gtfo::plus plus_obj;
    GTFO_TEST_ASSERT_EQ(plus_obj(std::string("Hello "), "world"), "Hello world");
    { gtfo::plus::is_transparent * p = nullptr; (void)p; }

    GTFO_TEST_ASSERT_EQ(gtfo::minus()(4, 3), 1);
    const gtfo::minus minus_obj;
    GTFO_TEST_ASSERT_NEAR_E6(minus_obj(4.5, 3), 1.5);
    { gtfo::minus::is_transparent * p = nullptr; (void)p; }

    GTFO_TEST_ASSERT_EQ(gtfo::multiplies()(4, 3), 12);
    const gtfo::multiplies multiplies_obj;
    GTFO_TEST_ASSERT_NEAR_E6(multiplies_obj(4.5, 3), 13.5);
    { gtfo::multiplies::is_transparent * p = nullptr; (void)p; }

    GTFO_TEST_ASSERT_EQ(gtfo::divides()(5, 2), 2);
    const gtfo::divides divides_obj;
    GTFO_TEST_ASSERT_NEAR_E6(divides_obj(5.f, 2.f), 2.5f);
    { gtfo::divides::is_transparent * p = nullptr; (void)p; }

    GTFO_TEST_ASSERT_EQ(gtfo::modulus()(5, 2), 1);
    const gtfo::modulus modulus_obj;
    GTFO_TEST_ASSERT_EQ(modulus_obj(15ull, 6u), 3ull);
    { gtfo::modulus::is_transparent * p = nullptr; (void)p; }

    GTFO_TEST_ASSERT_EQ(gtfo::negate()(5), -5);
    const gtfo::negate negate_obj;
    GTFO_TEST_ASSERT_NEAR_E6(negate_obj(5.0l), -5.0l);
    { gtfo::negate::is_transparent * p = nullptr; (void)p; }
GTFO_TEST_FUN_END
