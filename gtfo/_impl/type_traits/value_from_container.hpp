#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_VALUE_FROM_CONTAINER_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_VALUE_FROM_CONTAINER_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/is_container.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename C, bool c_is_container>
            struct impl_value_from_container
            {
            };

            template<typename C>
            struct impl_value_from_container<C, true>
            {
                typedef typename decay
                        <
                            decltype( * begin(declval<C &>()) )
                        >::type type;
            };
        }

        /// declares member type which tells
        /// values of what type does a container of type T store
        /// (T may also be reference to container);
        /// if is_container<T>::value == false, no member type is provided
        template<typename T>
        struct value_from_container : helpers::impl_value_from_container<T, is_container<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_VALUE_FROM_CONTAINER_HPP
