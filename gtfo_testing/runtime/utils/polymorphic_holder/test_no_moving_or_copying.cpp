#include "gtfo/utils/polymorphic_holder.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

namespace
{
    class Derived;

    class Base
    {
    public:
        virtual Derived * as_Derived() noexcept { return nullptr; }
        virtual ~Base() noexcept { }
    };

    class Derived : public Base
    {
    private:
        int _value;
    public:
        explicit Derived(int value) noexcept : _value(value) { }
        Derived * as_Derived() noexcept override { return this; }
        int value() const noexcept { return _value; }
    };

    using scoped_base_holder = gtfo::polymorphic_holder<Base, 16, 8, gtfo::polymorphic_holder_MCP::no_moving_or_copying, gtfo::polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        scoped_base_holder holder;
        GTFO_TEST_ASSERT(!holder);
        holder.construct<Derived>(42);
        GTFO_TEST_ASSERT(holder);
        GTFO_TEST_ASSERT(holder->as_Derived() != nullptr);
        GTFO_TEST_ASSERT(holder->as_Derived()->value() == 42);

        holder.reset();
        GTFO_TEST_ASSERT(!holder);
        holder.reset<Base>();
        GTFO_TEST_ASSERT(holder && !(holder->as_Derived()));
        holder.reset<Derived>(99);
        GTFO_TEST_ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 99));
    }
}

GTFO_TEST_FUN_BEGIN
    ::run_test();
GTFO_TEST_FUN_END
