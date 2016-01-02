#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ITERATOR_OF_RANGE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ITERATOR_OF_RANGE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_range.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename R, bool r_is_range>
            struct impl_iterator_of_range
            {
            };

            template<typename R>
            struct impl_iterator_of_range<R, true>
            {
                typedef typename decay
                        <
                            decltype( begin(declval<R &>()) )
                        >::type type;
            };
        }

        /// declares member type which tells
        /// iterators of what type does a range of type T use
        /// (T may also be a type of reference to such range);
        /// if is_range<T>::value == false, no member type is provided
        template<typename T>
        struct iterator_of_range : helpers::impl_iterator_of_range<T, is_range<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ITERATOR_OF_RANGE_HPP
