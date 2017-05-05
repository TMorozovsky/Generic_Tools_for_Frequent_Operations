#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// type T is dereferenceable, i.e. expression
        ///     * lvalue-of-type-T
        /// is well-formed
        template<typename T>
        struct is_dereferenceable
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( * declval<U &>() )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP
