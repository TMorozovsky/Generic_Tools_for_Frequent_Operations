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

    using scoped_base_holder = polymorphic_holder<Base, 16, 8, polymorphic_holder_MCP::no_moving_or_copying, polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        scoped_base_holder holder;
        ASSERT(!holder);
        holder.construct<Derived>(42);
        ASSERT(holder);
        ASSERT(holder->as_Derived() != nullptr);
        ASSERT(holder->as_Derived()->value() == 42);

        holder.reset();
        ASSERT(!holder);
        holder.reset<Base>();
        ASSERT(holder && !(holder->as_Derived()));
        holder.reset<Derived>(99);
        ASSERT(holder && holder->as_Derived() && (holder->as_Derived()->value() == 99));
    }
}

TEST_FUN_BEGIN
    ::run_test();
TEST_FUN_END
