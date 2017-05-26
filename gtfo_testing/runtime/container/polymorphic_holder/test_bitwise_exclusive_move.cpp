#include "gtfo/container/polymorphic_holder.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

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

        virtual int dynamic_value() const override { GTFO_TEST_ASSERT(_p_obj); return _p_obj->value(); }
    };

    using base_holder = gtfo::polymorphic_holder<Base, 24, 8, gtfo::polymorphic_holder_MCP::bitwise_exclusive_move, gtfo::polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        base_holder holder;
        GTFO_TEST_ASSERT(!holder);
        holder.construct<Derived>(42);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        GTFO_TEST_ASSERT(holder && holder->dynamic_value() == 42);

        base_holder holder_moved(std::move(holder));
        GTFO_TEST_ASSERT(holder_moved && holder_moved->dynamic_value() == 42);
        GTFO_TEST_ASSERT(!holder);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        static_assert(noexcept(base_holder(std::move(holder))),   "invalid noexcept-specification");
        static_assert(noexcept(holder = std::move(holder_moved)), "invalid noexcept-specification");

        holder = std::move(holder_moved);
        GTFO_TEST_ASSERT(holder && holder->dynamic_value() == 42);
        GTFO_TEST_ASSERT(!holder_moved);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        holder.reset();
        GTFO_TEST_ASSERT(!holder);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 0u);

        holder.reset<Derived>(99);
        GTFO_TEST_ASSERT(holder && holder->dynamic_value() == 99);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        auto other_holder = base_holder::make();
        GTFO_TEST_ASSERT(!other_holder);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);

        other_holder = base_holder::make<Derived>(100);
        GTFO_TEST_ASSERT(other_holder && other_holder->dynamic_value() == 100);
        GTFO_TEST_ASSERT_EQ(DynamicallyAllocatedObject::count(), 2u);
    }
}

GTFO_TEST_FUN_BEGIN
    GTFO_TEST_ASSERT_EQ(::DynamicallyAllocatedObject::count(), 0u);
    ::run_test();
    GTFO_TEST_ASSERT_EQ(::DynamicallyAllocatedObject::count(), 0u);
GTFO_TEST_FUN_END
