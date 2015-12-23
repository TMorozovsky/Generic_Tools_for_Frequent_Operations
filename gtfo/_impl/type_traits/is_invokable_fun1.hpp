#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN1_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN1_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an lvalue of type UnaryOperation
        /// can be invoked with an rvalue of type Argument
        template<typename UnaryOperation, typename Argument>
        struct is_invokable_fun1
        {
            template<typename Fun, typename Arg>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Fun &>()(declval<Arg>()) )
                                 >::type *);

            template<typename Fun, typename Arg>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test
                                                      <
                                                          UnaryOperation,
                                                          Argument
                                                      >(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN1_HPP
