#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INCREMENTABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INCREMENTABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// type T is incrementable, i.e. expression
        ///     ++ lvalue-of-type-T
        /// is well-formed
        template<typename T>
        struct is_incrementable
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( ++ declval<U &>() )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type)
#ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
                                               && !is_void<typename remove_reference<T>::type>::value
                                               && !is_const<typename remove_reference<T>::type>::value
#endif
                    ;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INCREMENTABLE_HPP
