#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ITERATOR_OF_CONTAINER_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ITERATOR_OF_CONTAINER_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/is_container.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename C, bool c_is_container>
            struct impl_iterator_of_container
            {
            };

            template<typename C>
            struct impl_iterator_of_container<C, true>
            {
                typedef typename decay
                        <
                            decltype( begin(declval<C &>()) )
                        >::type type;
            };
        }

        /// declares member type which tells
        /// iterators of what type does a container of type T use;
        /// if is_container<T>::value == false, no member type is provided
        template<typename T>
        struct iterator_of_container : helpers::impl_iterator_of_container<T, is_container<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ITERATOR_OF_CONTAINER_HPP
