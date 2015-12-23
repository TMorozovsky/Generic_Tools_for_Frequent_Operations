#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_ITERATOR_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/is_incrementable.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"
#include "gtfo/_impl/type_traits/is_equality_comparable.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if an lvalue of type T
        /// can be treated as a valid iterator,
        /// meaning that it can at least be incremented,
        /// dereferenced (returning non-void result)
        /// and equality-compared to other lvalues of this type
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
