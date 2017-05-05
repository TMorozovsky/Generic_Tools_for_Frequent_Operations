#include "polymorphic_holder.hpp"
#include "testing/testing.hpp"

namespace
{
    class DynamicallyAllocatedObject
    {
    private:
        static std::atomic<unsigned> _count;

    private:
        int _value;

    public:
        static unsigned count() noexcept { return _count; }

        int value() const noexcept { return _value; }

        explicit DynamicallyAllocatedObject(int value) noexcept : _value(value) { ++_count; }
        ~DynamicallyAllocatedObject() noexcept { --_count; }

        DynamicallyAllocatedObject(const DynamicallyAllocatedObject &) = delete;
    };
    std::atomic<unsigned> DynamicallyAllocatedObject::_count(0);

    class Base
    {
    public:
        virtual int dynamic_value() const = 0;

        virtual ~Base() noexcept { }
    };

    class Derived : public Base
    {
    private:
        simple_unique_ptr<DynamicallyAllocatedObject> _p_obj;

    public:
        explicit Derived(int value) : _p_obj(new DynamicallyAllocatedObject(value)) { }

        virtual int dynamic_value() const override { ASSERT(_p_obj); return _p_obj->value(); }
    };

    using base_holder = polymorphic_holder<Base, 24, 8, polymorphic_holder_MCP::bitwise_exclusive_move, polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        base_holder holder;
        ASSERT(!holder);
        holder.construct<Derived>(42);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        ASSERT(holder && holder->dynamic_value() == 42);

        base_holder holder_moved(std::move(holder));
        ASSERT(holder_moved && holder_moved->dynamic_value() == 42);
        ASSERT(!holder);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        static_assert(noexcept(base_holder(std::move(holder))),   "invalid noexcept-specification");
        static_assert(noexcept(holder = std::move(holder_moved)), "invalid noexcept-specification");

        holder = std::move(holder_moved);
        ASSERT(holder && holder->dynamic_value() == 42);
        ASSERT(!holder_moved);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        holder.reset();
        ASSERT(!holder);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 0u);

        holder.reset<Derived>(99);
        ASSERT(holder && holder->dynamic_value() == 99);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        auto other_holder = base_holder::make();
        ASSERT(!other_holder);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        other_holder = base_holder::make<Derived>(100);
        ASSERT(other_holder && other_holder->dynamic_value() == 100);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 2u);
    }
}

TEST_FUN_BEGIN
    ASSERT_EQ(::DynamicallyAllocatedObject::count(), 0u);
    ::run_test();
    ASSERT_EQ(::DynamicallyAllocatedObject::count(), 0u);
TEST_FUN_END
