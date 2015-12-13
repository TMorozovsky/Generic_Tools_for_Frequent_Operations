#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP

#include "_impl/type_traits/_common_definitions.hpp"
#include "_impl/type_traits/is_incrementable.hpp"
#include "_impl/type_traits/is_dereferenceable.hpp"
#include "_impl/type_traits/is_equality_comparable.hpp"
#include "_impl/type_traits/value_of_dereferenced.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// object of type T can be treated as a valid iterator
        /// that can be (at least) dereferenced, incremented,
        /// equality-compared to another object of this type,
        /// and its dereferencing result is not void
        template<typename T>
        struct is_iterator
        {
            template<typename U, bool u_is_incrementable_dereferenceable_and_comparable>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = !is_void
                                                   <
                                                       typename value_of_dereferenced<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   is_incrementable<T>::value &&
                                                   is_dereferenceable<T>::value &&
                                                   is_equality_comparable<T, T>::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP
