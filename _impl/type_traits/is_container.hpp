#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_CONTAINER_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_CONTAINER_HPP

#include "_impl/type_traits/_common_definitions.hpp"
#include "_impl/type_traits/has_begin.hpp"
#include "_impl/type_traits/has_end.hpp"
#include "_impl/type_traits/result_of_begin.hpp"
#include "_impl/type_traits/result_of_end.hpp"
#include "_impl/type_traits/is_iterator.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            /// defines static member constant value of type bool
            /// which is true if and only if
            /// a call to
            ///     begin(object-of-type-T)
            /// returns a valid iterator
            template<typename T>
            struct has_iterator_returning_begin
            {
                template<typename U, bool u_has_begin>
                struct impl
                {
                    static GTFO_CONSTEXPR bool value = false;
                };

                template<typename U>
                struct impl<U, true>
                {
                    static GTFO_CONSTEXPR bool value = is_iterator
                                                       <
                                                           typename result_of_begin<U>::type
                                                       >::value;
                };

                static GTFO_CONSTEXPR bool value = impl
                                                   <
                                                       T,
                                                       has_begin<T>::value
                                                   >::value;
            };

            /// defines static member constant value of type bool
            /// which is true if and only if
            /// a call to
            ///     end(object-of-type-T)
            /// returns a valid iterator
            template<typename T>
            struct has_iterator_returning_end
            {
                template<typename U, bool u_has_end>
                struct impl
                {
                    static GTFO_CONSTEXPR bool value = false;
                };

                template<typename U>
                struct impl<U, true>
                {
                    static GTFO_CONSTEXPR bool value = is_iterator
                                                       <
                                                           typename result_of_end<U>::type
                                                       >::value;
                };

                static GTFO_CONSTEXPR bool value = impl
                                                   <
                                                       T,
                                                       has_end<T>::value
                                                   >::value;
            };
        }

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// object of type T is a valid container,
        /// i.e. both calls to
        ///     begin(object-of-type-T)
        /// and
        ///     end(object-of-type-T)
        /// return valid iterators of the same type
        template<typename T>
        struct is_container
        {
            template<typename U, bool u_has_iterator_returning_begin_and_end>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = is_same
                                                   <
                                                       typename result_of_begin<U>::type,
                                                       typename result_of_end<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   helpers::has_iterator_returning_begin<T>::value &&
                                                   helpers::has_iterator_returning_end<T>::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_CONTAINER_HPP
