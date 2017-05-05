#include "gtfo/utils/polymorphic_holder.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

namespace
{
    class Base
    {
    public:
        int                value                = 1;
        const int          const_value          = 2;
        volatile int       volatile_value       = 3;
        const volatile int const_volatile_value = 4;

        virtual ~Base() { }
    };

    class Derived : public Base
    {
    };

    using test_holder = gtfo::polymorphic_holder<Base, 64, 8, gtfo::polymorphic_holder_MCP::no_moving_or_copying, gtfo::polymorphic_holder_DRP::bytes_array_only>;

    using pm_value_t                = decltype(&Base::value);
    using pm_const_value_t          = decltype(&Base::const_value);
    using pm_volatile_value_t       = decltype(&Base::volatile_value);
    using pm_const_volatile_value_t = decltype(&Base::const_volatile_value);

    volatile pm_value_t                g_pm_value                = &Base::value               ;
    volatile pm_const_value_t          g_pm_const_value          = &Base::const_value         ;
    volatile pm_volatile_value_t       g_pm_volatile_value       = &Base::volatile_value      ;
    volatile pm_const_volatile_value_t g_pm_const_volatile_value = &Base::const_volatile_value;

    void run_test()
    {
        const pm_value_t                pm_value                = g_pm_value               ;
        const pm_const_value_t          pm_const_value          = g_pm_const_value         ;
        const pm_volatile_value_t       pm_volatile_value       = g_pm_volatile_value      ;
        const pm_const_volatile_value_t pm_const_volatile_value = g_pm_const_volatile_value;

        test_holder h;
        h.construct<Derived>();

        GTFO_TEST_ASSERT_EQ(h->*pm_value, 1);
        h->*pm_value = 11;
        GTFO_TEST_ASSERT_EQ(h->*pm_value, 11);

        GTFO_TEST_ASSERT_EQ(h->*pm_const_value, 2);

        GTFO_TEST_ASSERT_EQ(h->*pm_volatile_value, 3);
        h->*pm_volatile_value = 33;
        GTFO_TEST_ASSERT_EQ(h->*pm_volatile_value, 33);

        GTFO_TEST_ASSERT_EQ(h->*pm_const_volatile_value, 4);

        const test_holder & ch = h;

        GTFO_TEST_ASSERT_EQ(ch->*pm_value, 11);
        GTFO_TEST_ASSERT_EQ(ch->*pm_const_value, 2);
        GTFO_TEST_ASSERT_EQ(ch->*pm_volatile_value, 33);
        GTFO_TEST_ASSERT_EQ(ch->*pm_const_volatile_value, 4);
    }
}

GTFO_TEST_FUN_BEGIN
    ::run_test();
GTFO_TEST_FUN_END
