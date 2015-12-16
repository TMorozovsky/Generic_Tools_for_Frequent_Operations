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
            template<typename T, bool t_is_container>
            struct impl_result_of_container_iterator_dereferencing
            {
            };

            template<typename T>
            struct impl_result_of_container_iterator_dereferencing<T, true>
            {
                typedef typename result_of_dereferencing
                        <
                            typename add_lvalue_reference
                            <
                                typename iterator_of_container
                                <
                                    typename remove_reference
                                    <
                                        T
                                    >::type
                                >::type
                            >::type
                        >::type type;
            };
        }


        /// declares member type which is the type returned by
        /// dereferencing operation applied to an lvalue iterator
        /// from a container of type T
        /// (where T is T_or_TRef with top-level reference stripped);
        /// if T is not a container, no member type is provided
        template<typename T_or_TRef>
        struct result_of_container_iterator_dereferencing : helpers::impl_result_of_container_iterator_dereferencing
                                                            <
                                                                typename remove_reference<T_or_TRef>::type,
                                                                is_container
                                                                <
                                                                    typename remove_reference<T_or_TRef>::type
                                                                >::value
                                                            >
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_CONTAINER_ITERATOR_DEREFERENCING_HPP

