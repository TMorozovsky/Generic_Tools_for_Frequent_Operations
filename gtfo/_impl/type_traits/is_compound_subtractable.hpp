#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_COMPOUND_SUBTRACTABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_COMPOUND_SUBTRACTABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            struct is_compound_subtractable_tester
            {
                template<typename Lhs, typename Rhs>
                static yes_type test(typename remove_reference<
                                         decltype( declval<Lhs &>() -= declval<Rhs>() )
                                     >::type *);

                template<typename Lhs, typename Rhs>
                static no_type test(...);
            };
        }

        // Defines a static member constant value of type bool
        // which is true if the expression
        //     lvalue-of-type-Lhs -= rvalue-of-type-Rhs
        // is well-formed.
        template<typename Lhs, typename Rhs>
        struct is_compound_subtractable
        {
            static constexpr bool value = sizeof(detail::is_compound_subtractable_tester::test<Lhs, Rhs>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_COMPOUND_SUBTRACTABLE_HPP
