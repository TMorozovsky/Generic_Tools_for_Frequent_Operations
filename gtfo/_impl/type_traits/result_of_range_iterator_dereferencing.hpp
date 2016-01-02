#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_RANGE_ITERATOR_DEREFERENCING_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_RANGE_ITERATOR_DEREFERENCING_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_range.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, bool t_is_range>
            struct impl_result_of_range_iterator_dereferencing
            {
            };

            template<typename T>
            struct impl_result_of_range_iterator_dereferencing<T, true>
            {
                typedef typename result_of_dereferencing
                        <
                            typename iterator_of_range< T >::type &
                        >::type type;
            };
        }


        /// declares member type which is the type returned by
        /// dereferencing operation applied to an lvalue iterator
        /// from a range of type T
        /// (T can also be type of reference to some range);
        /// if T is neither range nor reference to range,
        /// no member type is provided
        template<typename T>
        struct result_of_range_iterator_dereferencing : helpers::impl_result_of_range_iterator_dereferencing
                                                        <
                                                            T,
                                                            is_range<T>::value
                                                        >
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_RANGE_ITERATOR_DEREFERENCING_HPP

