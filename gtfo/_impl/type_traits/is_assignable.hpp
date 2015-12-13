#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ASSIGNABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ASSIGNABLE_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an expression of type From can be assigned
        /// to an expression of type To
        template<typename To, typename From>
        struct is_assignable
        {

            template<typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Lhs>() = declval<Rhs>() )
                                 >::type *);

            template<typename Lhs, typename Rhs>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<To, From>(nullptr)) == sizeof(yes_type)
#ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
                                               && !is_const<typename remove_reference<To>::type>::value
#endif
                    ;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ASSIGNABLE_HPP

