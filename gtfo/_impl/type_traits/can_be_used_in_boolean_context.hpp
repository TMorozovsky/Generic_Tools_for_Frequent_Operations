#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_CAN_BE_USED_IN_BOOLEAN_CONTEXT_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_CAN_BE_USED_IN_BOOLEAN_CONTEXT_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if an rvalue of type T
        /// can be used in a boolean context;
        /// note:
        /// in MSVC 2012, there is a bug which answers "true"
        /// if T's boolean cast operator is private
        template<typename T>
        struct can_be_used_in_boolean_context
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( static_cast<bool>(declval<T>()) )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };

#ifdef GTFO_NEED_WORKAROUNDS_FOR_MSVC_CLANG
        template<> struct can_be_used_in_boolean_context<void>                { static constexpr bool value = false; };
        template<> struct can_be_used_in_boolean_context<const void>          { static constexpr bool value = false; };
        template<> struct can_be_used_in_boolean_context<volatile void>       { static constexpr bool value = false; };
        template<> struct can_be_used_in_boolean_context<const volatile void> { static constexpr bool value = false; };
#endif
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_CAN_BE_USED_IN_BOOLEAN_CONTEXT_HPP

