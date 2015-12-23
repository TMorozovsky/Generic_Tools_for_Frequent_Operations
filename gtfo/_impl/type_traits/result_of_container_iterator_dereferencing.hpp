#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_CONTAINER_ITERATOR_DEREFERENCING_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_CONTAINER_ITERATOR_DEREFERENCING_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/is_container.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_container.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, bool t_is_container_or_reference_to_container>
            struct impl_result_of_container_iterator_dereferencing
            {
            };

            template<typename T>
            struct impl_result_of_container_iterator_dereferencing<T, true>
            {
                typedef typename result_of_dereferencing
                        <
                            typename iterator_of_container< T >::type &
                        >::type type;
            };
        }


        /// declares member type which is the type returned by
        /// dereferencing operation applied to an lvalue iterator
        /// from a container of type T
        /// (T can also be type of reference to some container);
        /// if T is neither container nor reference to container,
        /// no member type is provided
        template<typename T>
        struct result_of_container_iterator_dereferencing : helpers::impl_result_of_container_iterator_dereferencing
                                                            <
                                                                T,
                                                                is_container<T>::value
                                                            >
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_CONTAINER_ITERATOR_DEREFERENCING_HPP

