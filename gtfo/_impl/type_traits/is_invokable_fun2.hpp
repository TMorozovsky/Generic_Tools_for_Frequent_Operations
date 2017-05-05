#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN2_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN2_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an lvalue of type BinaryOperation
        /// can be invoked with two rvalues of types
        /// LhsArgument and RhsArgument
        template<typename BinaryOperation, typename LhsArgument, typename RhsArgument>
        struct is_invokable_fun2
        {
            template<typename Fun, typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Fun &>()(declval<Lhs>(), declval<Rhs>()) )
                                 >::type *);

            template<typename Fun, typename Lhs, typename Rhs>
            static no_type test(...);

            static constexpr bool value = sizeof(test
                                                 <
                                                     BinaryOperation,
                                                     LhsArgument,
                                                     RhsArgument
                                                 >(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN2_HPP

