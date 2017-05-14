#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_NOTHROW_LOGICALLY_NEGATABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_NOTHROW_LOGICALLY_NEGATABLE_HPP

#include "gtfo/_impl/type_traits/is_logically_negatable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<typename Argument, bool Argument_is_logically_negatable>
            struct is_nothrow_logically_negatable_impl;

            template<typename Argument>
            struct is_nothrow_logically_negatable_impl<Argument, false>
            {
                static constexpr bool value = false;
            };

            template<typename Argument>
            struct is_nothrow_logically_negatable_impl<Argument, true>
            {
                static constexpr bool value = noexcept( !::gtfo::_tt::declval<Argument>() );
            };
        }

        // Defines a static member constant value of type bool which is true if and only if
        // an rvalue of type T can be logically negated using the ! operator
        // and such logical negation cannot throw exceptions.
        template<typename Argument>
        struct is_nothrow_logically_negatable
        {
            static constexpr bool value = detail::is_nothrow_logically_negatable_impl< Argument,
                                                                                       is_logically_negatable<Argument>::value
                                                                                     >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_NOTHROW_LOGICALLY_NEGATABLE_HPP
