#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_PREDICATE2_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_PREDICATE2_HPP

#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an lvalue of type BinaryPredicate
        /// can be invoked with two rvalues
        /// of types Argument1 and Argument2,
        /// returning something that can be used in a boolean context
        template<typename BinaryPredicate, typename Argument1, typename Argument2>
        struct is_predicate2
        {
            template<typename F, typename T, typename U, bool f_is_invokable_with_t_and_u>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename F, typename T, typename U>
            struct impl<F, T, U, true>
            {
                static GTFO_CONSTEXPR bool value = can_be_used_in_boolean_context
                                                   <
                                                       typename result_of_fun2
                                                       <
                                                           F,
                                                           T,
                                                           U
                                                       >::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   BinaryPredicate,
                                                   Argument1,
                                                   Argument2,
                                                   is_invokable_fun2
                                                   <
                                                       BinaryPredicate,
                                                       Argument1,
                                                       Argument2
                                                   >::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_PREDICATE2_HPP
