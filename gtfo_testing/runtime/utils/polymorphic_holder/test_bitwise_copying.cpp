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

    using base_holder = gtfo::polymorphic_holder<Base, 16, 8, gtfo::polymorphic_holder_MCP::bitwise_copying, gtfo::polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        base_holder holder;
        GTFO_TEST_ASSERT(!holder);
        holder.construct<Derived>(42);
        GTFO_TEST_ASSERT(holder);
        GTFO_TEST_ASSERT(holder->as_Derived() != nullptr);
        GTFO_TEST_ASSERT(holder->as_Derived()->value() == 42);

        base_holder holder_copy(holder);
        GTFO_TEST_ASSERT(holder_copy && holder_copy->as_Derived() && (holder_copy->as_Derived()->value() == 42));

        static_assert(noexcept(base_holder(std::move(holder))),  "invalid noexcept-specification");
        static_assert(noexcept(base_holder(holder)),             "invalid noexcept-specification");
        static_assert(noexcept(holder = std::move(holder_copy)), "invalid noexcept-specification");
        static_assert(noexcept(holder = holder_copy),            "invalid noexcept-specification");

        base_holder holder_moved(std::move(holder));
        GTFO_TEST_ASSERT(holder_moved && holder_moved->as_Derived() && (holder_moved->as_Derived()->value() == 42));
        // despite being moved-from, the object should still exist and have the same value
        // because we use bitwise_copying as our MCP
        GTFO_TEST_ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 42));

        holder = std::move(holder_copy);
        GTFO_TEST_ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 42));
        GTFO_TEST_ASSERT(holder_copy && holder_copy->as_Derived() && (holder_copy->as_Derived()->value() == 42));

        holder.reset();
        GTFO_TEST_ASSERT(!holder);
        holder.reset<Base>();
        GTFO_TEST_ASSERT(holder && !(holder->as_Derived()));
        holder.reset<Derived>(99);
        GTFO_TEST_ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 99));

        holder = base_holder::make();
        GTFO_TEST_ASSERT(!holder);
        holder = base_holder::make<Derived>(100);
        GTFO_TEST_ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 100));
    }
}

GTFO_TEST_FUN_BEGIN
    ::run_test();
GTFO_TEST_FUN_END
