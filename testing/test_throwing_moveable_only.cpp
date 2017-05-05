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

    class Base : public polymorphic_holder_utils::throwing_moveable_only
    {
    public:
        virtual bool has_dynamic_value() const = 0;
        virtual int dynamic_value() const = 0;
        virtual void make_dynamic_value(int new_value) = 0;

        virtual ~Base() noexcept { }
    };

    class Derived : public Base
    {
    private:
        simple_unique_ptr<DynamicallyAllocatedObject> _p_obj;

        static std::atomic<unsigned> _num_successful_moves;
        static std::atomic<unsigned> _num_failed_moves;

    public:
        class moving_exception
        {
        };

    public:
        explicit Derived(int value) : _p_obj(new DynamicallyAllocatedObject(value)) { }

        Derived(Derived && other)
            : _p_obj(nullptr)
        {
            if ((_num_successful_moves + _num_failed_moves) % 2 == 1) {
                // every second move operation will throw
                ++_num_failed_moves;
                throw moving_exception();
            } else {
                _p_obj = std::move(other._p_obj);
                ++_num_successful_moves;
            }
        }

        POLYMORPHIC_HOLDER_THROWING_MOVEABLE_ONLY(Derived)

        virtual bool has_dynamic_value() const override { return static_cast<bool>(_p_obj); }
        virtual int dynamic_value() const override { ASSERT(_p_obj); return _p_obj->value(); }
        virtual void make_dynamic_value(int new_value) override { _p_obj.reset(new DynamicallyAllocatedObject(new_value)); }

        static unsigned num_successful_moves() noexcept { return _num_successful_moves; }
        static unsigned num_failed_moves() noexcept { return _num_failed_moves; }
    };
    std::atomic<unsigned> Derived::_num_successful_moves;
    std::atomic<unsigned> Derived::_num_failed_moves;

    using base_holder = polymorphic_holder<Base, 24, 8, polymorphic_holder_MCP::throwing_moveable_only, polymorphic_holder_DRP::bytes_array_only>;

    void run_test()
    {
        base_holder holder;
        ASSERT(!holder);
        holder.construct<Derived>(42);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        ASSERT(holder && holder->dynamic_value() == 42);

        base_holder holder_moved(std::move(holder)); // first move should not throw
        ASSERT(holder_moved && holder_moved->dynamic_value() == 42);
        ASSERT(holder && !holder->has_dynamic_value());
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        ASSERT_EQ(Derived::num_successful_moves(), 1u);
        ASSERT_EQ(Derived::num_failed_moves(), 0u);

        static_assert(!noexcept(base_holder(std::move(holder))),   "invalid noexcept-specification");
        static_assert(!noexcept(holder = std::move(holder_moved)), "invalid noexcept-specification");

        holder->make_dynamic_value(100);
        ASSERT_EQ(DynamicallyAllocatedObject::count(), 2u);

        try {
            holder = std::move(holder_moved); // second move should not throw
            ASSERT(!"should not have reached here!");
        } catch (const Derived::moving_exception &) {
            ASSERT_EQ(Derived::num_failed_moves(), 1u);
            ASSERT(holder_moved && holder_moved->dynamic_value() == 42); // source object should have been left unchanged...
            ASSERT(!holder); // ...but the destination object was left empty
            ASSERT_EQ(DynamicallyAllocatedObject::count(), 1u);
        }
    }
}

TEST_FUN_BEGIN
    ASSERT_EQ(::DynamicallyAllocatedObject::count(), 0u);
    ::run_test();
    ASSERT_EQ(::DynamicallyAllocatedObject::count(), 0u);
TEST_FUN_END
