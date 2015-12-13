#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_MULTIPLICATION_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_MULTIPLICATION_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/are_multipliable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename Lhs, typename Rhs, bool lhs_and_rhs_are_multipliable>
            struct impl_result_of_multiplication
            {
            };

            template<typename Lhs, typename Rhs>
            struct impl_result_of_multiplication<Lhs, Rhs, true>
            {
                typedef decltype( declval<Lhs>() * declval<Rhs>() ) type;
            };
        }

        /// declares member type which is the type of expression
        ///     rvalue-of-type-LhsArgument * rvalue-of-type-RhsArgument;
        /// if such expression is ill-formed, no member type is provided
        template<typename LhsArgument, typename RhsArgument>
        struct result_of_multiplication : helpers::impl_result_of_multiplication<LhsArgument, RhsArgument,
                                                                                 are_multipliable<LhsArgument, RhsArgument>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_MULTIPLICATION_HPP

