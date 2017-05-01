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

    class Base : public polymorphic_holder_utils::nothrow_moveable_only
    {
    public:
        virtual bool has_dynamic_value() const = 0;
        virtual int dynamic_value() const = 0;

        virtual ~Base() noexcept { }
    };

    class Derived : public Base
    {
    private:
        simple_unique_ptr<DynamicallyAllocatedObject> _p_obj;

        static std::atomic<unsigned> _num_moves;

    public:
        explicit Derived(int value) : _p_obj(new DynamicallyAllocatedObject(value)) { }

        Derived(Derived && other) noexcept
            : _p_obj(std::move(other._p_obj))
        {
            ++_num_moves;
        }

        POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_ONLY(Derived)

        virtual bool has_dynamic_value() const override { return static_cast<bool>(_p_obj); }
        virtual int dynamic_value() const override { ASSERT(_p_obj); return _p_obj->value(); }

        static unsigned num_moves() noexcept { return _num_moves; }
    };
    std::atomic<unsigned> Derived::_num_moves;

    using base_holder = polymorphic_holder<Base, 24, 8, polymorphic_holder_MCP::nothrow_moveable_only, polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        base_holder holder;
        ASSERT(!holder);
        ASSERT_EQ(Derived::num_moves(), 0u);
        holder.construct<Derived>(42);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        ASSERT(holder && holder->dynamic_value() == 42);

        base_holder holder_moved(std::move(holder));
        ASSERT(holder_moved && holder_moved->dynamic_value() == 42);
        ASSERT(holder && !holder->has_dynamic_value());
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        ASSERT_EQ(Derived::num_moves(), 1u);

        holder = std::move(holder_moved);
        ASSERT(holder && holder->dynamic_value() == 42);
        ASSERT(holder_moved && !holder_moved->has_dynamic_value());
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        ASSERT_EQ(Derived::num_moves(), 2u);

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
