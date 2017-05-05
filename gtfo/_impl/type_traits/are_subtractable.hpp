#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_SUBTRACTABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_SUBTRACTABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if an expression
        ///     lvalue-of-type-LhsArgument - lvalue-of-type-RhsArgument
        /// is well-formed
        template<typename LhsArgument, typename RhsArgument>
        struct are_subtractable
        {
            template<typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Lhs &>() - declval<Rhs &>() )
                                 >::type *);

            template< typename Lhs, typename Rhs>
            static no_type test(...);

            static constexpr bool value = sizeof(test
                                                 <
                                                     LhsArgument,
                                                     RhsArgument
                                                 >(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_SUBTRACTABLE_HPP

