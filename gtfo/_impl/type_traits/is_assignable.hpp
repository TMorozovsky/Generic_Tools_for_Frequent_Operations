#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ASSIGNABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ASSIGNABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an rvalue of type From can be assigned
        /// to an lvalue of type To
        /// note:
        /// unlike the is_assignable<> type trait from the standard library,
        /// this implementation always treats its first parameter as if
        /// it was an lvalue, even if the actual argument was an rvalue
        template<typename To, typename From>
        struct is_assignable
        {

            template<typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Lhs &>() = declval<Rhs>() )
                                 >::type *);

            template<typename Lhs, typename Rhs>
            static no_type test(...);

            static constexpr bool value = sizeof(test<To, From>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ASSIGNABLE_HPP
