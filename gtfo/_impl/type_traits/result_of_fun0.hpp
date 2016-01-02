#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN0_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN0_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_invokable_fun0.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename Fun, bool fun_is_invokable>
            struct impl_result_of_fun0
            {
            };

            template<typename Fun>
            struct impl_result_of_fun0<Fun, true>
            {
                typedef decltype( declval<Fun &>()() ) type;
            };
        }

        /// declares member type which is the type returned by
        /// a function call operator of an lvalue of type Function
        /// without any explicitly provided arguments;
        /// if such call is ill-formed, no member type is provided
        template<typename Function, typename Argument>
        struct result_of_fun0 : helpers::impl_result_of_fun0<Function,
                                                             is_invokable_fun0<Function>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN0_HPP
