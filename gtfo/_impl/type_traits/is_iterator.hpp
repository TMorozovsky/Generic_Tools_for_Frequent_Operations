#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_incrementable.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_n_eq.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"

namespace gtfo
{
    namespace _tt
    {
        // !!! DEPRECATED !!!
        template<typename T>
        struct is_iterator
        {
            template<typename U, bool u_is_incrementable_dereferenceable_and_comparable>
            struct impl
            {
                static constexpr bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static constexpr bool value = !is_void
                                              <
                                                  typename value_of_dereferenced<U>::type
                                              >::value;
            };

            static constexpr bool value = impl
                                          <
                                              T,
                                              is_incrementable<T>::value &&
                                              is_dereferenceable<T>::value &&
                                              are_comparable_op_eq<T, T>::value &&
                                              are_comparable_op_n_eq<T, T>::value
                                          >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP
