#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_COMMON_TYPE_2_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_COMMON_TYPE_2_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T1, typename T2>
            struct common_type_2_exists
            {
                template<typename U1, typename U2>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( true ? declval<U1>() : declval<U2>() )
                                     >::type *);

                template<typename U1, typename U2>
                static no_type test(...);

                static GTFO_CONSTEXPR bool value = sizeof(test<T1, T2>(nullptr)) == sizeof(yes_type);
            };

            template<typename T1, typename T2, bool t1_and_t2_have_a_common_type>
            struct impl_common_type_2
            {
            };

            template<typename T1, typename T2>
            struct impl_common_type_2<T1, T2, true>
            {
                typedef typename decay
                        <
                            decltype( true ? declval<T1>() : declval<T2>() )
                        >::type type;
            };
        }

        /// declares member type which is the type
        /// both T1 and T2 can be implicitly converted to
        /// (top-level reference and cv-qualifiers are stripped);
        /// if no common type exists, no member type is provided
        template<typename T1, typename T2>
        struct common_type_2 : helpers::impl_common_type_2<T1, T2,
                                                           helpers::common_type_2_exists<T1, T2>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_COMMON_TYPE_2_HPP

