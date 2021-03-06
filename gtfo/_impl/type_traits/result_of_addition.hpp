#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_ADDITION_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_ADDITION_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/are_addable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename Lhs, typename Rhs, bool lhs_and_rhs_are_addable>
            struct impl_result_of_addition
            {
            };

            template<typename Lhs, typename Rhs>
            struct impl_result_of_addition<Lhs, Rhs, true>
            {
                typedef decltype( declval<Lhs &>() + declval<Rhs &>() ) type;
            };
        }

        /// declares member type which is the type of expression
        ///     lvalue-of-type-LhsArgument + lvalue-of-type-RhsArgument;
        /// if such expression is ill-formed, no member type is provided
        template<typename LhsArgument, typename RhsArgument>
        struct result_of_addition : helpers::impl_result_of_addition<LhsArgument, RhsArgument,
                                                                     are_addable<LhsArgument, RhsArgument>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_ADDITION_HPP
