#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_DEREFERENCING_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_DEREFERENCING_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<typename T, bool t_is_dereferenceable>
            struct result_of_dereferencing_impl
            {
            };

            template<typename T>
            struct result_of_dereferencing_impl<T, true>
            {
                typedef decltype( * declval<T &>() ) type;
            };
        }

        // Declares member type which is the type of expression
        //     * lvalue-of-type-T
        // stored "as is", without stripping any top-level reference or cv-qualifiers.
        // If is_dereferenceable<T>::value == false, no member type is provided.
        template<typename T>
        struct result_of_dereferencing : detail::result_of_dereferencing_impl< T,
                                                                               is_dereferenceable<T>::value >
        {
        };

        template<typename T>
        using result_of_dereferencing_t = typename result_of_dereferencing<T>::type;
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_DEREFERENCING_HPP
