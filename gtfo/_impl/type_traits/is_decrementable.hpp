#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DECREMENTABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DECREMENTABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            struct is_decrementable_tester
            {
                template<typename T>
                static yes_type test(typename remove_reference<
                                         decltype( -- declval<T &>() )
                                     >::type *);

                template<typename T>
                static no_type test(...);
            };
        }

        // Defines a static member constant value of type bool
        // which is true if type T is decrementable, i.e. if the expression
        //     -- lvalue-of-type-T
        // is well-formed.
        template<typename T>
        struct is_decrementable
        {
            static constexpr bool value = sizeof(detail::is_decrementable_tester::test<T>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DECREMENTABLE_HPP
