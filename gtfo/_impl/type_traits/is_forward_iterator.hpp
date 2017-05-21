#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_FORWARD_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_FORWARD_ITERATOR_HPP

#include "gtfo/_impl/type_traits/is_input_iterator.hpp"
#include "gtfo/_impl/type_traits/is_output_iterator.hpp"

namespace gtfo
{
    namespace _tt
    {
        // Defines a static member constant value of type bool which is true
        // if type T can be treated as a valid forward iterator.
        template<typename T>
        struct is_forward_iterator
        {
            static constexpr bool value = is_input_iterator<T>::value &&
                                          is_default_constructible<T>::value;
        };

        // Defines a static member constant value of type bool which is true
        // if type T can be treated as a valid forward iterator
        // that allows modification of its dereferencing result.
        template<typename T>
        struct is_mutable_forward_iterator
        {
            static constexpr bool value = is_forward_iterator<T>::value &&
                                          is_output_iterator<T>::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_FORWARD_ITERATOR_HPP
