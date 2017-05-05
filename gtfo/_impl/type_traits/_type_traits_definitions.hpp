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
        using ::std::is_fundamental;
        using ::std::remove_reference;
        using ::std::is_lvalue_reference;
        using ::std::decay;

        using ::std::declval;

        typedef char(&yes_type)[1];
        typedef char(&no_type) [2];

        template<typename T>
        struct is_polymorphic
        {
            static_assert(sizeof(T) > 0, "incomplete types are not allowed");

#ifdef GTFO_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
            static constexpr bool value = __is_polymorphic(T);
#else
            static constexpr bool value = ::std::is_polymorphic<T>::value;
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

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP
