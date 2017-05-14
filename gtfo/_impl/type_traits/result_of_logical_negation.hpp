#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_LOGICAL_NEGATION_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_LOGICAL_NEGATION_HPP

#include "gtfo/_impl/type_traits/is_logically_negatable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<typename Argument, bool Argument_is_logically_negatable>
            struct result_of_logical_negation_impl
            {
            };

            template<typename Argument>
            struct result_of_logical_negation_impl<Argument, true>
            {
                using type = decltype( !::gtfo::_tt::declval<Argument>() );
            };
        }

        // Declares a member type which is the type of expression
        //     ! rvalue-of-type-Argument
        // (if such expression is ill-formed, no member type is provided).
        template<typename Argument>
        struct result_of_logical_negation : detail::result_of_logical_negation_impl< Argument, is_logically_negatable<Argument>::value >
        {
        };

        template<typename Argument>
        using result_of_logical_negation_t = typename result_of_logical_negation<Argument>::type;
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_LOGICAL_NEGATION_HPP
