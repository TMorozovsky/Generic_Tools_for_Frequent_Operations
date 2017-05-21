#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INPUT_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INPUT_ITERATOR_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_incrementable.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_n_eq.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<typename T, bool T_satisfies_basic_requirements>
            struct is_input_iterator_impl
            {
                static constexpr bool value = false;
            };

            template<typename T>
            struct is_input_iterator_impl<T, true>
            {
                static constexpr bool value = !is_void< value_of_dereferenced_t<T> >::value;
            };
        }

        // Defines a static member constant value of type bool
        // which is true if type T can be treated as a valid input iterator.
        template<typename T>
        struct is_input_iterator
        {
            static constexpr bool value = detail::is_input_iterator_impl< T,
                                                                          is_copy_constructible<T>::value &&
                                                                          is_copy_assignable<T>::value &&
                                                                          is_destructible<T>::value &&
                                                                          is_incrementable<T>::value &&
                                                                          is_dereferenceable<T>::value &&
                                                                          are_comparable_op_eq<T, T>::value &&
                                                                          are_comparable_op_n_eq<T, T>::value >::value;
        };

        // References are not iterators.
        template<typename T>
        struct is_input_iterator<T &>
        {
            static constexpr bool value = false;
        };
        template<typename T>
        struct is_input_iterator<T &&>
        {
            static constexpr bool value = false;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INPUT_ITERATOR_HPP
