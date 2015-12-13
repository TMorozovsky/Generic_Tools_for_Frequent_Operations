#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_END_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_END_HPP

#include "_impl/type_traits/_common_definitions.hpp"
#include "_impl/type_traits/has_end.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, bool t_has_end>
            struct impl_result_of_end
            {
            };

            template<typename T>
            struct impl_result_of_end<T, true>
            {
                typedef typename decay
                        <
                            decltype( end(declval<T &>()) )
                        >::type type;
            };
        }

        /// declares member type which is the type returned from
        ///     end(object-of-type-T);
        /// top-level reference and cv-qualifiers (if any) are stripped;
        /// if has_end<T>::value == false, no member type is provided
        template<typename T>
        struct result_of_end : helpers::impl_result_of_end<T, has_end<T>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_END_HPP
