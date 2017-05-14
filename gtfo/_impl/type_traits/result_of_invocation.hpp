#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_INVOCATION_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_INVOCATION_HPP

#include "gtfo/_impl/type_traits/is_invokable.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( push )
//  Disable MSVC Compiler Warning C4244 ("'argument' : conversion from 'type1' to 'type2', possible loss of data")
#   pragma warning( disable : 4244 )
#endif

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<bool Fun_is_invokable_with_Args, typename Fun, typename... Args>
            struct result_of_invocation_impl
            {
            };

            template<typename Fun, typename... Args>
            struct result_of_invocation_impl<true, Fun, Args...>
            {
                using type = decltype( ::gtfo::_tt::declval<Fun>()(::gtfo::_tt::declval<Args>()...) );
            };
        }

        // Declares a member type which is the type returned by a function call operator
        // applied to an rvalue of type Function with rvalue arguments of types Arguments
        // (if such call is ill-formed, no member type is provided).
        template<typename Function, typename... Arguments>
        struct result_of_invocation : detail::result_of_invocation_impl< is_invokable<Function, Arguments...>::value,
                                                                         Function,
                                                                         Arguments... >
        {
        };

        template<typename Function, typename... Arguments>
        using result_of_invocation_t = typename result_of_invocation<Function, Arguments...>::type;
    }
}

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_INVOCATION_HPP
