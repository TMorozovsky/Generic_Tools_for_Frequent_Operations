#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_CAN_APPLY_ARRAY_SUBSCRIPT_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_CAN_APPLY_ARRAY_SUBSCRIPT_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            struct can_apply_array_subscript_tester
            {
                template<typename A, typename S>
                static yes_type test(typename remove_reference<
                                         decltype( declval<A &>()[declval<S>()] )
                                     >::type *);

                template<typename A, typename S>
                static no_type test(...);
            };
        }

        // Defines a static member constant value of type bool
        // which is true if the expression
        //     lvalue-of-type-A [ rvalue-of-type-S ]
        // is well-formed.
        template<typename A, typename S>
        struct can_apply_array_subscript
        {
            static constexpr bool value = sizeof(detail::can_apply_array_subscript_tester::test<A, S>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_CAN_APPLY_ARRAY_SUBSCRIPT_HPP
