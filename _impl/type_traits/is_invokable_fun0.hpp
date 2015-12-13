#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN0_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN0_HPP

#include "_impl/type_traits/_common_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// Function is a type of function or function object
        /// that can be invoked without any arguments
        template<typename Function>
        struct is_invokable_fun0
        {
            template<typename Fun>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Fun &>()() )
                                 >::type *);

            template<typename Fun>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<Function>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_INVOKABLE_FUN0_HPP
