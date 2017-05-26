#include "gtfo/container/polymorphic_holder.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( push )
//  Disable MSVC Compiler Warning C4324 ("'struct_name': structure was padded due to alignment specifier")
#   pragma warning( disable : 4324 )
#endif

namespace
{
    class alignas(16) Base : public gtfo::polymorphic_holder_utils::nothrow_moveable_and_nothrow_copyable
    {
    public:
        virtual int mutable_value() const volatile = 0;
        virtual void multiply_mutable_value_by(int arg) const volatile = 0;

        virtual ~Base() noexcept { }
    };

    struct DummyBase
    {
        char dummy_buffer[1024 - 16];

        virtual void bar() = 0;

        virtual ~DummyBase() noexcept { }
    };

    class Derived : public DummyBase, public Base
    {
    public:
        struct copying_exception
        {
        };

    private:
        mutable int _value;

    public:
        explicit Derived(int value) : _value(value) { }

        GTFO_POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_NOTHROW_COPYABLE(Derived)

        virtual void bar() override { this->dummy_buffer[0] = '\0'; }

        virtual int mutable_value() const volatile override { return _value; }
        virtual void multiply_mutable_value_by(int arg) const volatile override { _value *= arg; }
    };

    using base_holder = gtfo::polymorphic_holder<const Base, 1200, 16, gtfo::polymorphic_holder_MCP::nothrow_moveable_and_nothrow_copyable, gtfo::polymorphic_holder_DRP::offset_after_bytes>;

    void run_test()
    {
        {
            std::size_t offset = gtfo::polymorphic_holder_lib::get_offset_to_base_from_derived<Base, Derived>();
            constexpr std::size_t expected_offset = 1024;
            GTFO_TEST_ASSERT_EQ(offset, expected_offset);
        }

        base_holder holder;
        GTFO_TEST_ASSERT(!holder);
        holder.construct<const Derived>(42);
        GTFO_TEST_ASSERT(holder && holder->mutable_value() == 42);

        base_holder holder_moved(std::move(holder));
        GTFO_TEST_ASSERT(holder_moved && holder_moved->mutable_value() == 42);

        static_assert(noexcept(base_holder(std::move(holder))),   "invalid noexcept-specification");
        static_assert(noexcept(base_holder(holder)),              "invalid noexcept-specification");
        static_assert(noexcept(holder = std::move(holder_moved)), "invalid noexcept-specification");
        static_assert(noexcept(holder = holder_moved),            "invalid noexcept-specification");

        holder = std::move(holder_moved);
        GTFO_TEST_ASSERT(holder && holder->mutable_value() == 42);

        holder.reset();
        GTFO_TEST_ASSERT(!holder);

        holder.reset<const Derived>(99);
        GTFO_TEST_ASSERT(holder && holder->mutable_value() == 99);

        auto other_holder = base_holder::make();
        GTFO_TEST_ASSERT(!other_holder);

        other_holder = base_holder::make<Derived>(100);
        GTFO_TEST_ASSERT(other_holder && other_holder->mutable_value() == 100);

        base_holder holder_copy(other_holder);
        GTFO_TEST_ASSERT(holder_copy && holder_copy->mutable_value() == 100);

        other_holder = holder;
        GTFO_TEST_ASSERT(other_holder && other_holder->mutable_value() == 99);
    }
}

GTFO_TEST_FUN_BEGIN
    ::run_test();
GTFO_TEST_FUN_END

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif
