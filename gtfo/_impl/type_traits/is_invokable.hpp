#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            struct is_invokable_impl
            {
                template<typename Fun, typename... Args>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( ::gtfo::_tt::declval<Fun>()(::gtfo::_tt::declval<Args>()...) )
                                     >::type *);

                template<typename Fun, typename... Args>
                static no_type test(...);
            };
        }

        // Defines a static member constant value of type bool which is true if and only if
        // an rvalue of type Function is invokable via its operator ()
        // with rvalue arguments of types Arguments.
        template<typename Function, typename... Arguments>
        struct is_invokable
        {
            static constexpr bool value = sizeof(detail::is_invokable_impl::test<Function, Arguments...>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_HPP
