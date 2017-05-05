#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN2_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN2_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_invokable_fun2.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( push )
//  Disable MSVC Compiler Warning C4244 ("'argument' : conversion from 'type1' to 'type2', possible loss of data")
#   pragma warning( disable : 4244 )
#endif

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
        /// a call to lvalue of type BinaryOperation
        /// with two rvalue arguments of types Argument1 and Argument2;
        /// if such call is ill-formed, no member type is provided
        template<typename BinaryOperation, typename Argument1, typename Argument2>
        struct result_of_fun2 : helpers::impl_result_of_fun2<BinaryOperation, Argument1, Argument2,
                                                             is_invokable_fun2<BinaryOperation, Argument1, Argument2>::value>
        {
        };
    }
}

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_RESULT_OF_FUN2_HPP
