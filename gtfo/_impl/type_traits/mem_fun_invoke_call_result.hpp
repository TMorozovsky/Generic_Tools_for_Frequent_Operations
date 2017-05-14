#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_CALL_RESULT_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_CALL_RESULT_HPP

#include "gtfo/_impl/type_traits/mem_fun_invoke_is_callable.hpp"

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
            template<bool Objects_mem_fun_invoke_is_callable_with_Args, typename Object, typename... Args>
            struct mem_fun_invoke_call_result_impl
            {
            };

            template<typename Object, typename... Args>
            struct mem_fun_invoke_call_result_impl<true, Object, Args...>
            {
                using type = decltype( ::gtfo::_tt::declval<Object>().invoke(::gtfo::_tt::declval<Args>()...) );
            };
        }

        // Declares a member type which is the type returned by (possibly, cv-qualified or ref-qualified) Object's member function "invoke()"
        // with rvalue arguments of types Arguments (if such call is ill-formed, no member type is provided).
        template<typename Object, typename... Arguments>
        struct mem_fun_invoke_call_result : detail::mem_fun_invoke_call_result_impl< mem_fun_invoke_is_callable<Object, Arguments...>::value,
                                                                                     Object,
                                                                                     Arguments... >
        {
        };

        template<typename Object, typename... Arguments>
        using mem_fun_invoke_call_result_t = typename mem_fun_invoke_call_result<Object, Arguments...>::type;
    }
}

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_MEM_FUN_INVOKE_CALL_RESULT_HPP
