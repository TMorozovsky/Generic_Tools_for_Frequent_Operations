#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_HAS_END_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_HAS_END_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     end(object-of-type-T)
        /// is well-formed
        template<typename T>
        struct has_end
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( end(declval<U &>()) )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_HAS_END_HPP