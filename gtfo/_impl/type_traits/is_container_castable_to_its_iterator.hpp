#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/is_container.hpp"
#include "gtfo/_impl/type_traits/iterator_of_container.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// lvalue of type T is a valid container
        /// (or a reference to such container) that is
        /// castable to the type of that container's iterator
        template<typename T>
        struct is_container_castable_to_its_iterator
        {
            template<typename U, bool u_is_container>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                typedef typename iterator_of_container<U>::type Iterator;

                template<typename V>
                static yes_type test(typename remove_reference
                                     <
                                        decltype( Iterator(declval<V &>()) )
                                     >::type *);

                template<typename V>
                static no_type test(...);

                static GTFO_CONSTEXPR bool value = sizeof(test<U>(nullptr)) == sizeof(yes_type);
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   is_container<T>::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR_HPP

