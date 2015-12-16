#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN2_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN2_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"
#include "gtfo/_impl/type_traits/is_invokable_fun2.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename Fun, typename Arg1, typename Arg2, bool fun_is_invokable>
            struct impl_result_of_fun2
            {
            };

            template<typename Fun, typename Arg1, typename Arg2>
            struct impl_result_of_fun2<Fun, Arg1, Arg2, true>
            {
                typedef decltype( declval<Fun &>()(declval<Arg1>(), declval<Arg2>()) ) type;
            };
        }

        /// declares member type which is the type returned by
        /// a call to BinaryOperation function (function object)
        /// with two arguments of types Argument1 and Argument2;
        /// if such call is ill-formed, no member type is provided
        template<typename BinaryOperation, typename Argument1, typename Argument2>
        struct result_of_fun2 : helpers::impl_result_of_fun2<BinaryOperation, Argument1, Argument2,
                                                             is_invokable_fun2<BinaryOperation, Argument1, Argument2>::value>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN2_HPP