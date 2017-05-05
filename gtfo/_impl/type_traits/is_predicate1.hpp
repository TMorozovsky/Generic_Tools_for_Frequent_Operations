#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_PREDICATE1_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_PREDICATE1_HPP

#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"
#include "gtfo/_impl/type_traits/result_of_fun1.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an lvalue of type UnaryPredicate
        /// can be invoked with one rvalue of type Argument,
        /// returning something that can be used in a boolean context
        template<typename UnaryPredicate, typename Argument>
        struct is_predicate1
        {
            template<typename F, typename T, bool f_is_invokable_with_t>
            struct impl
            {
                static constexpr bool value = false;
            };

            template<typename F, typename T>
            struct impl<F, T, true>
            {
                static constexpr bool value = can_be_used_in_boolean_context
                                              <
                                                  typename result_of_fun1
                                                  <
                                                      F,
                                                      T
                                                  >::type
                                              >::value;
            };

            static constexpr bool value = impl
                                          <
                                              UnaryPredicate,
                                              Argument,
                                              is_invokable_fun1
                                              <
                                                  UnaryPredicate,
                                                  Argument
                                              >::value
                                          >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_PREDICATE1_HPP
