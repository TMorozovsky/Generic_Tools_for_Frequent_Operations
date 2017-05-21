#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_BIDIRECTIONAL_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_BIDIRECTIONAL_ITERATOR_HPP

#include "gtfo/_impl/type_traits/is_forward_iterator.hpp"
#include "gtfo/_impl/type_traits/is_decrementable.hpp"

namespace gtfo
{
    namespace _tt
    {
        // Defines a static member constant value of type bool which is true
        // if type T can be treated as a valid bidirectional iterator.
        template<typename T>
        struct is_bidirectional_iterator
        {
            static constexpr bool value = is_forward_iterator<T>::value &&
                                          is_decrementable<T>::value;
        };

        // Defines a static member constant value of type bool which is true
        // if type T can be treated as a valid bidirectional iterator
        // that allows modification of its dereferencing result.
        template<typename T>
        struct is_mutable_bidirectional_iterator
        {
            static constexpr bool value = is_mutable_forward_iterator<T>::value &&
                                          is_decrementable<T>::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_BIDIRECTIONAL_ITERATOR_HPP
