#include "gtfo/functional/not_.hpp"
#include "gtfo/_impl/type_traits/result_of_invocation.hpp"
#include "gtfo/_impl/type_traits/is_nothrow_invokable.hpp"

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct E { };
    struct F { };
    struct G { };
    struct H { };

    template<typename X>
    struct Result
    {
        X operator ! () const volatile
            noexcept(noexcept(X()) && noexcept(X(X())))
        { return X(); }
    };

    struct Foo
    {
        Result<A> operator () (int)                &  { return Result<A>(); }
        Result<B> operator () (int) const          &  { return Result<B>(); }
        Result<C> operator () (int) volatile       &  { return Result<C>(); }
        Result<D> operator () (int) const volatile &  { return Result<D>(); }

        Result<E> operator () (int)                && { return Result<E>(); }
        Result<F> operator () (int) const          && { return Result<F>(); }
        Result<G> operator () (int) volatile       && { return Result<G>(); }
        Result<H> operator () (int) const volatile && { return Result<H>(); }
    };

    template<typename Object, typename... Args>
    constexpr unsigned int get_noexceptness_mask()
    {
        return 0u |
               (gtfo::_tt::is_nothrow_invokable<               Object &,  Args...>::value ? (1u << 0) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<const          Object &,  Args...>::value ? (1u << 1) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<volatile       Object &,  Args...>::value ? (1u << 2) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<const volatile Object &,  Args...>::value ? (1u << 3) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<               Object &&, Args...>::value ? (1u << 4) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<const          Object &&, Args...>::value ? (1u << 5) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<volatile       Object &&, Args...>::value ? (1u << 6) : 0u) |
               (gtfo::_tt::is_nothrow_invokable<const volatile Object &&, Args...>::value ? (1u << 7) : 0u);
    }

    static_assert(get_noexceptness_mask<Foo, int &>() == 0, "");
}

using gtfo::_tt::is_same;
using gtfo::_tt::result_of_invocation_t;

using NotFoo = gtfo::not_t<Foo>;

static_assert(is_same<result_of_invocation_t<NotFoo &,                int>, A>::value, "");
static_assert(is_same<result_of_invocation_t<const NotFoo &,          int>, B>::value, "");
static_assert(is_same<result_of_invocation_t<volatile NotFoo &,       int>, C>::value, "");
static_assert(is_same<result_of_invocation_t<const volatile NotFoo &, int>, D>::value, "");

static_assert(is_same<result_of_invocation_t<NotFoo,                  int>, E>::value, "");
static_assert(is_same<result_of_invocation_t<const NotFoo,            int>, F>::value, "");
static_assert(is_same<result_of_invocation_t<volatile NotFoo,         int>, G>::value, "");
static_assert(is_same<result_of_invocation_t<const volatile NotFoo,   int>, H>::value, "");

namespace
{
    struct Test1
    {
        Result<A> operator () (int)                & noexcept { return Result<A>(); }
        Result<B> operator () (int) const          &          { return Result<B>(); }
        Result<C> operator () (int) volatile       &          { return Result<C>(); }
        Result<D> operator () (int) const volatile &          { return Result<D>(); }

        Result<E> operator () (int)                &&         { return Result<E>(); }
        Result<F> operator () (int) const          &&         { return Result<F>(); }
        Result<G> operator () (int) volatile       &&         { return Result<G>(); }
        Result<H> operator () (int) const volatile &&         { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test1, int>() == 1u, "");

    struct Test2
    {
        Result<A> operator () (int)                &          { return Result<A>(); }
        Result<B> operator () (int) const          & noexcept { return Result<B>(); }
        Result<C> operator () (int) volatile       &          { return Result<C>(); }
        Result<D> operator () (int) const volatile &          { return Result<D>(); }

        Result<E> operator () (int)                &&         { return Result<E>(); }
        Result<F> operator () (int) const          &&         { return Result<F>(); }
        Result<G> operator () (int) volatile       &&         { return Result<G>(); }
        Result<H> operator () (int) const volatile &&         { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test2, int>() == 2u, "");

    struct Test3
    {
        Result<A> operator () (int)                &          { return Result<A>(); }
        Result<B> operator () (int) const          &          { return Result<B>(); }
        Result<C> operator () (int) volatile       & noexcept { return Result<C>(); }
        Result<D> operator () (int) const volatile &          { return Result<D>(); }

        Result<E> operator () (int)                &&         { return Result<E>(); }
        Result<F> operator () (int) const          &&         { return Result<F>(); }
        Result<G> operator () (int) volatile       &&         { return Result<G>(); }
        Result<H> operator () (int) const volatile &&         { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test3, int>() == 4u, "");

    struct Test4
    {
        Result<A> operator () (int)                &          { return Result<A>(); }
        Result<B> operator () (int) const          &          { return Result<B>(); }
        Result<C> operator () (int) volatile       &          { return Result<C>(); }
        Result<D> operator () (int) const volatile & noexcept { return Result<D>(); }

        Result<E> operator () (int)                &&         { return Result<E>(); }
        Result<F> operator () (int) const          &&         { return Result<F>(); }
        Result<G> operator () (int) volatile       &&         { return Result<G>(); }
        Result<H> operator () (int) const volatile &&         { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test4, int>() == 8u, "");

    struct Test5
    {
        Result<A> operator () (int)                &           { return Result<A>(); }
        Result<B> operator () (int) const          &           { return Result<B>(); }
        Result<C> operator () (int) volatile       &           { return Result<C>(); }
        Result<D> operator () (int) const volatile &           { return Result<D>(); }

        Result<E> operator () (int)                && noexcept { return Result<E>(); }
        Result<F> operator () (int) const          &&          { return Result<F>(); }
        Result<G> operator () (int) volatile       &&          { return Result<G>(); }
        Result<H> operator () (int) const volatile &&          { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test5, int>() == 16u, "");

    struct Test6
    {
        Result<A> operator () (int)                &           { return Result<A>(); }
        Result<B> operator () (int) const          &           { return Result<B>(); }
        Result<C> operator () (int) volatile       &           { return Result<C>(); }
        Result<D> operator () (int) const volatile &           { return Result<D>(); }

        Result<E> operator () (int)                &&          { return Result<E>(); }
        Result<F> operator () (int) const          && noexcept { return Result<F>(); }
        Result<G> operator () (int) volatile       &&          { return Result<G>(); }
        Result<H> operator () (int) const volatile &&          { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test6, int>() == 32u, "");

    struct Test7
    {
        Result<A> operator () (int)                &           { return Result<A>(); }
        Result<B> operator () (int) const          &           { return Result<B>(); }
        Result<C> operator () (int) volatile       &           { return Result<C>(); }
        Result<D> operator () (int) const volatile &           { return Result<D>(); }

        Result<E> operator () (int)                &&          { return Result<E>(); }
        Result<F> operator () (int) const          &&          { return Result<F>(); }
        Result<G> operator () (int) volatile       && noexcept { return Result<G>(); }
        Result<H> operator () (int) const volatile &&          { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test7, int>() == 64u, "");

    struct Test8
    {
        Result<A> operator () (int)                &           { return Result<A>(); }
        Result<B> operator () (int) const          &           { return Result<B>(); }
        Result<C> operator () (int) volatile       &           { return Result<C>(); }
        Result<D> operator () (int) const volatile &           { return Result<D>(); }

        Result<E> operator () (int)                &&          { return Result<E>(); }
        Result<F> operator () (int) const          &&          { return Result<F>(); }
        Result<G> operator () (int) volatile       &&          { return Result<G>(); }
        Result<H> operator () (int) const volatile && noexcept { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test8, int>() == 128u, "");

    struct Test9
    {
        Result<A> operator () (int)                &           { return Result<A>(); }
        Result<B> operator () (int) const          &  noexcept { return Result<B>(); }
        Result<C> operator () (int) volatile       &           { return Result<C>(); }
        Result<D> operator () (int) const volatile &  noexcept { return Result<D>(); }

        Result<E> operator () (int)                &&          { return Result<E>(); }
        Result<F> operator () (int) const          &&          { return Result<F>(); }
        Result<G> operator () (int) volatile       && noexcept { return Result<G>(); }
        Result<H> operator () (int) const volatile &&          { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test9, int>() == 74u, "");

    struct Test10
    {
        Result<A> operator () (int)                &  noexcept { return Result<A>(); }
        Result<B> operator () (int) const          &  noexcept { return Result<B>(); }
        Result<C> operator () (int) volatile       &  noexcept { return Result<C>(); }
        Result<D> operator () (int) const volatile &  noexcept { return Result<D>(); }

        Result<E> operator () (int)                && noexcept { return Result<E>(); }
        Result<F> operator () (int) const          && noexcept { return Result<F>(); }
        Result<G> operator () (int) volatile       && noexcept { return Result<G>(); }
        Result<H> operator () (int) const volatile && noexcept { return Result<H>(); }
    };

    static_assert(get_noexceptness_mask<Test10, int>() == 255u, "");
}

namespace
{
    struct ConstexprTesterResult
    {
        int _value;

        constexpr ConstexprTesterResult() noexcept : _value(0) { }
        constexpr explicit ConstexprTesterResult(int value) noexcept : _value(value) { }
        constexpr ConstexprTesterResult(const ConstexprTesterResult & other) noexcept : _value(other._value) { }
        constexpr int operator ! () const noexcept { return _value; }
    };

    struct ConstexprTesterObject
    {
        int _value;

        constexpr ConstexprTesterObject() noexcept : _value(0) { }
        constexpr explicit ConstexprTesterObject(int value) noexcept : _value(value) { }
        constexpr ConstexprTesterObject(const ConstexprTesterObject & other) noexcept : _value(other._value) { }
        constexpr ConstexprTesterResult operator () () const noexcept { return ConstexprTesterResult(-_value); }
    };

    struct holder : gtfo::detail::functional::function_object_holder<ConstexprTesterObject>
    {
        using base = gtfo::detail::functional::function_object_holder<ConstexprTesterObject>;

        constexpr holder() noexcept : base() { }

        constexpr ConstexprTesterResult call() const noexcept { return this->base::invoke(); }
    };
    constexpr holder holder_instance = holder();
    constexpr ConstexprTesterResult holder_result = holder_instance.call();
    static_assert(!holder_result == 0, "");

    constexpr gtfo::not_t<ConstexprTesterObject> tester_1 { ConstexprTesterObject{100} };
    constexpr auto test_result_1 { tester_1() };
    static_assert(test_result_1 == -100, "");

    constexpr auto tester_2 = gtfo::not_(ConstexprTesterObject{150});
    constexpr int test_result_2 { tester_2() };
    static_assert(test_result_2 == -150, "");

#if defined(_MSC_VER) && !defined(__clang__) && (_MSC_VER <= 1900)
#   define GTFO_OLD_BAD_CONSTEXPR
#endif

#ifndef GTFO_OLD_BAD_CONSTEXPR
    constexpr int test_result_3 = gtfo::not_t<ConstexprTesterObject>(ConstexprTesterObject{200})();
    static_assert(test_result_3 == -200, "");

    constexpr int test_result_4 = gtfo::not_(ConstexprTesterObject{250})();
    static_assert(test_result_4 == -250, "");
#endif
}