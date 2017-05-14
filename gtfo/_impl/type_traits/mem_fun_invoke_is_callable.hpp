#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_IS_CALLABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_IS_CALLABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            struct mem_fun_invoke_is_callable_impl
            {
                template<typename Object, typename... Args>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( ::gtfo::_tt::declval<Object>().invoke(::gtfo::_tt::declval<Args>()...) )
                                     >::type *);

                template<typename Object, typename... Args>
                static no_type test(...);
            };
        }

        // Defines a static member constant value of type bool which is true if and only if
        // an rvalue of type Object has the a member function "invoke()"
        // which is callable with rvalue arguments of types Arguments.
        template<typename Object, typename... Arguments>
        struct mem_fun_invoke_is_callable
        {
            static constexpr bool value = sizeof(detail::mem_fun_invoke_is_callable_impl::test<Object, Arguments...>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_IS_CALLABLE_HPP
