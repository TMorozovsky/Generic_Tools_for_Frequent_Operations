#include "gtfo/utils/polymorphic_holder.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

#ifndef GTFO_POLYMORPHIC_HOLDER_DISABLE_OVERLOADED_PENDING_MEMBER_FUNCTION_CALL_OPERATOR

namespace
{
    class Base
    {
    public:
        virtual int f_simple               (int x, int y)                  = 0;
        virtual int f_const                (int x, int y) const            = 0;
        virtual int f_volatile             (int x, int y) volatile         = 0;
        virtual int f_const_volatile       (int x, int y) const volatile   = 0;

        virtual int f_simple_lvalue        (int x, int y) &                = 0;
        virtual int f_const_lvalue         (int x, int y) const &          = 0;
        virtual int f_volatile_lvalue      (int x, int y) volatile &       = 0;
        virtual int f_const_volatile_lvalue(int x, int y) const volatile & = 0;

        virtual void f_pass_by_ref         (int & x, int && y) const       = 0;
        virtual int & f_return_by_ref      (int & x) const noexcept        = 0;

        void f_nonvirtual_test             () const
        {
            volatile int tmp = this->f_const(5, 5);
            GTFO_TEST_ASSERT_EQ(tmp, 12);
            tmp = 10;
            GTFO_TEST_ASSERT_EQ(tmp, 10);
        }

        virtual ~Base() { }
    };

    class Derived : public Base
    {
    public:
        virtual int f_simple               (int x, int y)                  override { return x + y + 1; }
        virtual int f_const                (int x, int y) const            override { return x + y + 2; }
        virtual int f_volatile             (int x, int y) volatile         override { return x + y + 3; }
        virtual int f_const_volatile       (int x, int y) const volatile   override { return x + y + 4; }

        virtual int f_simple_lvalue        (int x, int y) &                override { return x + y + 5; }
        virtual int f_const_lvalue         (int x, int y) const &          override { return x + y + 6; }
        virtual int f_volatile_lvalue      (int x, int y) volatile &       override { return x + y + 7; }
        virtual int f_const_volatile_lvalue(int x, int y) const volatile & override { return x + y + 8; }

        virtual void f_pass_by_ref         (int & x, int && y) const       override { x = std::move(y); }
        virtual int & f_return_by_ref      (int & x) const noexcept        override { return x; }
    };

    using test_holder = gtfo::polymorphic_holder<Base, 16, 8, gtfo::polymorphic_holder_MCP::no_moving_or_copying, gtfo::polymorphic_holder_DRP::bytes_array_only>;

    using pmf_simple_t                = decltype(&Base::f_simple               );
    using pmf_const_t                 = decltype(&Base::f_const                );
    using pmf_volatile_t              = decltype(&Base::f_volatile             );
    using pmf_const_volatile_t        = decltype(&Base::f_const_volatile       );

    using pmf_simple_lvalue_t         = decltype(&Base::f_simple_lvalue        );
    using pmf_const_lvalue_t          = decltype(&Base::f_const_lvalue         );
    using pmf_volatile_lvalue_t       = decltype(&Base::f_volatile_lvalue      );
    using pmf_const_volatile_lvalue_t = decltype(&Base::f_const_volatile_lvalue);

    using pmf_pass_by_ref_t           = decltype(&Base::f_pass_by_ref          );
    using pmf_return_by_ref_t         = decltype(&Base::f_return_by_ref        );
    using pmf_nonvirtual_test_t       = decltype(&Base::f_nonvirtual_test      );

    volatile pmf_simple_t                g_pmf_simple                = &Base::f_simple               ;
    volatile pmf_const_t                 g_pmf_const                 = &Base::f_const                ;
    volatile pmf_volatile_t              g_pmf_volatile              = &Base::f_volatile             ;
    volatile pmf_const_volatile_t        g_pmf_const_volatile        = &Base::f_const_volatile       ;

    volatile pmf_simple_lvalue_t         g_pmf_simple_lvalue         = &Base::f_simple_lvalue        ;
    volatile pmf_const_lvalue_t          g_pmf_const_lvalue          = &Base::f_const_lvalue         ;
    volatile pmf_volatile_lvalue_t       g_pmf_volatile_lvalue       = &Base::f_volatile_lvalue      ;
    volatile pmf_const_volatile_lvalue_t g_pmf_const_volatile_lvalue = &Base::f_const_volatile_lvalue;

    volatile pmf_pass_by_ref_t           g_pmf_pass_by_ref           = &Base::f_pass_by_ref          ;
    volatile pmf_return_by_ref_t         g_pmf_return_by_ref         = &Base::f_return_by_ref        ;
    volatile pmf_nonvirtual_test_t       g_pmf_nonvirtual_test       = &Base::f_nonvirtual_test      ;

    volatile int                         g_lhs                       = 5;
    volatile int                         g_rhs                       = 5;

    void run_test()
    {
        const pmf_simple_t                pmf_simple                = g_pmf_simple               ;
        const pmf_const_t                 pmf_const                 = g_pmf_const                ;
        const pmf_volatile_t              pmf_volatile              = g_pmf_volatile             ;
        const pmf_const_volatile_t        pmf_const_volatile        = g_pmf_const_volatile       ;

        const pmf_simple_lvalue_t         pmf_simple_lvalue         = g_pmf_simple_lvalue        ;
        const pmf_const_lvalue_t          pmf_const_lvalue          = g_pmf_const_lvalue         ;
        const pmf_volatile_lvalue_t       pmf_volatile_lvalue       = g_pmf_volatile_lvalue      ;
        const pmf_const_volatile_lvalue_t pmf_const_volatile_lvalue = g_pmf_const_volatile_lvalue;

        const pmf_pass_by_ref_t           pmf_pass_by_ref           = g_pmf_pass_by_ref          ;
        const pmf_return_by_ref_t         pmf_return_by_ref         = g_pmf_return_by_ref        ;
        const pmf_nonvirtual_test_t       pmf_nonvirtual_test       = g_pmf_nonvirtual_test      ;

        const int lhs = g_lhs;
        const int rhs = g_rhs;

        test_holder h;
        h.construct<Derived>();

        int result_pmf_simple                = (h->*pmf_simple               )(lhs, rhs);
        int result_pmf_const                 = (h->*pmf_const                )(lhs, rhs);
        int result_pmf_volatile              = (h->*pmf_volatile             )(lhs, rhs);
        int result_pmf_const_volatile        = (h->*pmf_const_volatile       )(lhs, rhs);

        int result_pmf_simple_lvalue         = (h->*pmf_simple_lvalue        )(lhs, rhs);
        int result_pmf_const_lvalue          = (h->*pmf_const_lvalue         )(lhs, rhs);
        int result_pmf_volatile_lvalue       = (h->*pmf_volatile_lvalue      )(lhs, rhs);
        int result_pmf_const_volatile_lvalue = (h->*pmf_const_volatile_lvalue)(lhs, rhs);

        GTFO_TEST_ASSERT_EQ(result_pmf_simple               , 11);
        GTFO_TEST_ASSERT_EQ(result_pmf_const                , 12);
        GTFO_TEST_ASSERT_EQ(result_pmf_volatile             , 13);
        GTFO_TEST_ASSERT_EQ(result_pmf_const_volatile       , 14);

        GTFO_TEST_ASSERT_EQ(result_pmf_simple_lvalue        , 15);
        GTFO_TEST_ASSERT_EQ(result_pmf_const_lvalue         , 16);
        GTFO_TEST_ASSERT_EQ(result_pmf_volatile_lvalue      , 17);
        GTFO_TEST_ASSERT_EQ(result_pmf_const_volatile_lvalue, 18);

        int dest = 0;
        int src = 42;
        (h->*pmf_pass_by_ref)(dest, std::move(src));
        GTFO_TEST_ASSERT_EQ(dest, 42);
        int & ref_result = (h->*pmf_return_by_ref)(dest);
        GTFO_TEST_ASSERT_EQ(&ref_result, &dest);
        GTFO_TEST_ASSERT_EQ(ref_result, 42);

        (h->*pmf_nonvirtual_test)();
    }
}

GTFO_TEST_FUN_BEGIN
    ::run_test();
GTFO_TEST_FUN_END

#endif // !defined GTFO_POLYMORPHIC_HOLDER_DISABLE_OVERLOADED_PENDING_MEMBER_FUNCTION_CALL_OPERATOR
