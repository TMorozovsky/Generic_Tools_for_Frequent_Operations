#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_SUBTRACTION_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_SUBTRACTION_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/are_subtractable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename Lhs, typename Rhs, bool lhs_and_rhs_are_subtractable>
            struct impl_result_of_subtraction
            {
            };

            template<typename Lhs, typename Rhs>
            struct impl_result_of_subtraction<Lhs, Rhs, true>
            {
                typedef decltype( declval<Lhs &>() - declval<Rhs &>() ) type;
            };
        }

        /// declares member type which is the type of expression
        ///     lvalue-of-type-LhsArgument - lvalue-of-type-RhsArgument;
        /// if such expression is ill-formed, no member type is provided
        template<typename LhsArgument, typename RhsArgument>
        struct result_of_subtraction : helpers::impl_result_of_subtraction<LhsArgument, RhsArgument,
                                                                           are_subtractable<LhsArgument, RhsArgument>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_SUBTRACTION_HPP

