#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_BEGIN_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_BEGIN_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/has_begin.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, bool t_has_begin>
            struct impl_result_of_begin
            {
            };

            template<typename T>
            struct impl_result_of_begin<T, true>
            {
                typedef typename decay
                        <
                            decltype( begin(declval<T &>()) )
                        >::type type;
            };
        }

        /// declares member type which is the type returned from
        ///     begin(lvalue-of-type-T);
        /// top-level reference and cv-qualifiers (if any) are stripped;
        /// if has_begin<T>::value == false, no member type is provided
        template<typename T>
        struct result_of_begin : helpers::impl_result_of_begin<T, has_begin<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_BEGIN_HPP
