#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_IS_NOTHROW_CALLABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_IS_NOTHROW_CALLABLE_HPP

#include "gtfo/_impl/type_traits/mem_fun_invoke_is_callable.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<bool Objects_mem_fun_invoke_is_callable_with_Args, typename Object, typename... Args>
            struct mem_fun_invoke_is_nothrow_callable_impl;

            template<typename Object, typename... Args>
            struct mem_fun_invoke_is_nothrow_callable_impl<false, Object, Args...>
            {
                static constexpr bool value = false;
            };

            template<typename Object, typename... Args>
            struct mem_fun_invoke_is_nothrow_callable_impl<true, Object, Args...>
            {
                static constexpr bool value = noexcept( ::gtfo::_tt::declval<Object>().invoke(::gtfo::_tt::declval<Args>()...) );
            };
        }

        // Defines a static member constant value of type bool which is true if and only if
        // an rvalue of type Object has the a member function "invoke()"
        // which is callable with rvalue arguments of types Arguments
        // and such call is not allowed to throw exceptions.
        template<typename Object, typename... Arguments>
        struct mem_fun_invoke_is_nothrow_callable
        {
            static constexpr bool value = detail::mem_fun_invoke_is_nothrow_callable_impl< mem_fun_invoke_is_callable<Object, Arguments...>::value,
                                                                                           Object,
                                                                                           Arguments...
                                                                                         >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_IS_NOTHROW_CALLABLE_HPP
