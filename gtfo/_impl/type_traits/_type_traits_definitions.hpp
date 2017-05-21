#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP

#include "gtfo/_impl/_common_definitions.hpp"
#include <type_traits>

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUG__)
#   define GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
#endif

namespace gtfo
{
    namespace _tt
    {
        using ::std::enable_if;

        using ::std::is_same;
        using ::std::is_void;
        using ::std::is_const;
        using ::std::is_volatile;

        using ::std::is_arithmetic;
        using ::std::is_enum;
        using ::std::is_pointer;

        using ::std::is_constructible;
        using ::std::is_default_constructible;
        using ::std::is_copy_constructible;
        using ::std::is_move_constructible;
        using ::std::is_copy_assignable;
        using ::std::is_move_assignable;

        using ::std::is_nothrow_constructible;
        using ::std::is_nothrow_default_constructible;
        using ::std::is_nothrow_copy_constructible;
        using ::std::is_nothrow_move_constructible;
        using ::std::is_nothrow_assignable;
        using ::std::is_nothrow_copy_assignable;
        using ::std::is_nothrow_move_assignable;

        using ::std::is_destructible;

        using ::std::is_lvalue_reference;
        using ::std::is_rvalue_reference;
        using ::std::is_reference;

        using ::std::remove_const;
        using ::std::remove_reference;
        using ::std::decay;

        using ::std::declval;
        // The declval function template is not a type trait, but it is used in a simlar context most of the time,
        // so we define it here in order to keep the enclosing namespace a little bit cleaner.

        template<typename T> using remove_const_t     = typename ::gtfo::_tt::remove_const<T>::type;
        template<typename T> using remove_reference_t = typename ::gtfo::_tt::remove_reference<T>::type;
        template<typename T> using decay_t            = typename ::gtfo::_tt::decay<T>::type;

        typedef char(&yes_type)[1];
        typedef char(&no_type) [2];

        template<typename C>
        struct is_class
        {
#ifdef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
            static constexpr bool value = __is_class(C);
#else
            static constexpr bool value = ::std::is_class<C>::value;
#endif
        };

        template<typename C>
        struct is_union
        {
#ifdef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
            static constexpr bool value = __is_union(C);
#else
            static constexpr bool value = ::std::is_union<C>::value;
#endif
        };

        template<typename C>
        struct is_final
        {
            static_assert(sizeof(C) > 0, "incomplete types are not allowed");

#ifdef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
            static constexpr bool value = __is_final(C);
#elif __cplusplus > 201103L
            static constexpr bool value = ::std::is_final<C>::value;
#else
            // In the worst case, we can't determine whether a class is final, so
            // we answer "true" for every class to disable any attempts to inherit from it.
            static constexpr bool value = is_class<C>::value;
#endif
        };

        template<typename C>
        struct is_polymorphic
        {
            static_assert(sizeof(C) > 0, "incomplete types are not allowed");

#ifdef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
            static constexpr bool value = __is_polymorphic(C);
#else
            static constexpr bool value = ::std::is_polymorphic<C>::value;
#endif
        };

        template<typename B, typename D>
        struct is_base_of
        {
            static_assert(sizeof(B) > 0, "incomplete types are not allowed");
            static_assert(sizeof(D) > 0, "incomplete types are not allowed");

#ifdef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
            static constexpr bool value = __is_base_of(B, D);
#else
            static constexpr bool value = ::std::is_base_of<B, D>::value;
#endif
        };
    }
}

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUG__)
#   undef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
#endif

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP
