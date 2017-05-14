#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_LOGICALLY_NEGATABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_LOGICALLY_NEGATABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            struct is_logically_negatable_impl
            {
                template<typename T>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( !::gtfo::_tt::declval<T>() )
                                     >::type *);

                template<typename T>
                static no_type test(...);
            };
        }

        // Defines a static member constant value of type bool which is true if and only if
        // an rvalue of type T can be logically negated using the ! operator.
        template<typename T>
        struct is_logically_negatable
        {
            static constexpr bool value = sizeof(detail::is_logically_negatable_impl::test<T>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_LOGICALLY_NEGATABLE_HPP
