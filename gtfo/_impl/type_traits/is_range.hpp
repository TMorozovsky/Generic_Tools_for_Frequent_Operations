#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANGE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANGE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/has_begin.hpp"
#include "gtfo/_impl/type_traits/has_end.hpp"
#include "gtfo/_impl/type_traits/result_of_begin.hpp"
#include "gtfo/_impl/type_traits/result_of_end.hpp"
#include "gtfo/_impl/type_traits/is_iterator.hpp"
#include "gtfo/_impl/type_traits/is_equality_comparable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            /// defines static member constant value of type bool
            /// which is true if and only if
            /// a call to
            ///     begin(lvalue-of-type-T)
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
            ///     end(lvalue-of-type-T)
            /// returns some non-void value
            template<typename T>
            struct has_non_void_returning_end
            {
                template<typename U, bool u_has_end>
                struct impl
                {
                    static GTFO_CONSTEXPR bool value = false;
                };

                template<typename U>
                struct impl<U, true>
                {
                    static GTFO_CONSTEXPR bool value = !is_void
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
        /// T is a valid range type,
        /// i.e. if call to
        ///     begin(lvalue-of-type-T)
        /// returns a valid iterator (see is_iterator<> type trait)
        /// and call to
        ///     end(lvalue-of-type-T)
        /// returns something that can be equality-compared
        /// to that iterator in expressions like
        ///     begin(lvalue-of-type-T) == end(lvalue-of-type-T)
        /// and
        ///     begin(lvalue-of-type-T) != end(lvalue-of-type-T)
        template<typename T>
        struct is_range
        {
            template<typename U, bool u_has_iterator_returning_begin_non_void_returning_end>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = is_equality_comparable
                                                   <
                                                       typename result_of_begin<U>::type,
                                                       typename result_of_end<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   helpers::has_iterator_returning_begin<T>::value &&
                                                   helpers::has_non_void_returning_end<T>::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANGE_HPP
