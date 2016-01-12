#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_PRED_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_PRED_HPP

#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an lvalue of type BinaryPredicate can be used to compare
        /// lvalues of types LhsArgument and RhsArgument
        template<typename BinaryPredicate, typename LhsArgument, typename RhsArgument>
        struct are_comparable_pred
        {
            template<typename F, typename T, typename U, bool f_is_invokable_on_t_and_u>
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
                                                           T &,
                                                           U &
                                                       >::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   BinaryPredicate,
                                                   LhsArgument,
                                                   RhsArgument,
                                                   is_invokable_fun2
                                                   <
                                                       BinaryPredicate,
                                                       LhsArgument &,
                                                       RhsArgument &
                                                   >::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_EQ_HPP
