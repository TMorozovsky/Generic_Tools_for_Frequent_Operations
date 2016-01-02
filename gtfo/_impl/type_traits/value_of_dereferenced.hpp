#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_VALUE_OF_DEREFERENCED_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_VALUE_OF_DEREFERENCED_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, bool t_is_dereferenceable>
            struct impl_value_of_dereferenced
            {
            };

            template<typename T>
            struct impl_value_of_dereferenced<T, true>
            {
                typedef typename decay
                        <
                            decltype( * declval<T &>() )
                        >::type type;
            };
        }

        /// declares member type which is the type of expression
        ///     * lvalue-of-type-T
        /// with its top-level reference and cv-qualifiers stripped
        /// (as if it was returned or passed by value);
        /// if is_dereferenceable<T>::value == false, no member type is provided
        template<typename T>
        struct value_of_dereferenced : helpers::impl_value_of_dereferenced<T, is_dereferenceable<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_VALUE_OF_DEREFERENCED_HPP
