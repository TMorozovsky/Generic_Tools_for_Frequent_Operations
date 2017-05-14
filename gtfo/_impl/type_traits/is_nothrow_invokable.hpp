#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_NOTHROW_INVOKABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_NOTHROW_INVOKABLE_HPP

#include "gtfo/_impl/type_traits/is_invokable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<bool Function_is_invokable_with_Arguments, typename Function, typename... Arguments>
            struct is_nothrow_invokable_impl;

            template<typename Function, typename... Arguments>
            struct is_nothrow_invokable_impl<false, Function, Arguments...>
            {
                static constexpr bool value = false;
            };

            template<typename Fun, typename... Args>
            struct is_nothrow_invokable_impl<true, Fun, Args...>
            {
                static constexpr bool value = noexcept( ::gtfo::_tt::declval<Fun>()(::gtfo::_tt::declval<Args>()...) );
            };
        }

        // Defines a static member constant value of type bool which is true if and only if
        // an rvalue of type Function is invokable via its operator ()
        // with rvalue arguments of types Arguments
        // and such call is not allowed to throw exceptions.
        template<typename Function, typename... Arguments>
        struct is_nothrow_invokable
        {
            static constexpr bool value = detail::is_nothrow_invokable_impl< is_invokable<Function, Arguments...>::value,
                                                                             Function,
                                                                             Arguments...
                                                                           >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_NOTHROW_INVOKABLE_HPP
