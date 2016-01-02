#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN1_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN1_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_invokable_fun1.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename Fun, typename Arg, bool fun_is_invokable>
            struct impl_result_of_fun1
            {
            };

            template<typename Fun, typename Arg>
            struct impl_result_of_fun1<Fun, Arg, true>
            {
                typedef decltype( declval<Fun &>()(declval<Arg>()) ) type;
            };
        }

        /// declares member type which is the type returned by
        /// a call to lvalue of type UnaryOperation
        /// with one rvalue argument of type Argument;
        /// if such call is ill-formed, no member type is provided
        template<typename UnaryOperation, typename Argument>
        struct result_of_fun1 : helpers::impl_result_of_fun1<UnaryOperation, Argument,
                                                             is_invokable_fun1<UnaryOperation, Argument>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN1_HPP
