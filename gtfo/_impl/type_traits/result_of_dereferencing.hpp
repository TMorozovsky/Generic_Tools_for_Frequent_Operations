#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_DEREFERENCING_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_DEREFERENCING_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, bool t_is_dereferenceable>
            struct impl_result_of_dereferencing
            {
            };

            template<typename T>
            struct impl_result_of_dereferencing<T, true>
            {
                typedef decltype( * declval<T &>() ) type;
            };
        }

        /// declares member type which is the type of expression
        ///     * lvalue-of-type-T
        /// stored "as is", without stripping any top-level reference
        /// or cv-qualifiers;
        /// if is_dereferenceable<T>::value == false, no member type is provided
        template<typename T>
        struct result_of_dereferencing : helpers::impl_result_of_dereferencing<T, is_dereferenceable<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_DEREFERENCING_HPP
