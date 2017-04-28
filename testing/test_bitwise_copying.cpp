#include "polymorphic_holder.hpp"
#include "testing/testing.hpp"

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

    using base_holder = polymorphic_holder<Base, 16, 8, polymorphic_holder_MCP::bitwise_copying, polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        base_holder holder;
        ASSERT(!holder);
        holder.construct<Derived>(42);
        ASSERT(holder);
        ASSERT(holder->as_Derived() != nullptr);
        ASSERT(holder->as_Derived()->value() == 42);

        base_holder holder_copy(holder);
        ASSERT(holder_copy && holder_copy->as_Derived() && (holder_copy->as_Derived()->value() == 42));

        base_holder holder_moved(std::move(holder));
        ASSERT(holder_moved && holder_moved->as_Derived() && (holder_moved->as_Derived()->value() == 42));
        // despite being moved-from, the object should still exist and have the same value
        // because we use bitwise_copying as our MCP
        ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 42));

        holder = std::move(holder_copy);
        ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 42));
        ASSERT(holder_copy && holder_copy->as_Derived() && (holder_copy->as_Derived()->value() == 42));

        holder.reset();
        ASSERT(!holder);
        holder.reset<Base>();
        ASSERT(holder && !(holder->as_Derived()));
        holder.reset<Derived>(99);
        ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 99));

        holder = base_holder::make();
        ASSERT(!holder);
        holder = base_holder::make<Derived>(100);
        ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 100));
    }
}

TEST_FUN_BEGIN
    ::run_test();
TEST_FUN_END
