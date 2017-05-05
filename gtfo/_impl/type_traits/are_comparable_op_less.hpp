#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_LESS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_LESS_HPP

#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( push )
//  Disable MSVC Compiler Warning C4018 ("'expression' : signed/unsigned mismatch")
//  and Warning C4389 ("'operator' : signed/unsigned mismatch")
#   pragma warning( disable : 4018 )
#   pragma warning( disable : 4389 )
#endif

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, typename U>
            struct can_invoke_op_less
            {
                template<typename V, typename W, bool v_and_w_are_not_void>
                struct impl
                {
                    static constexpr bool value = false;
                };

                template<typename V, typename W>
                struct impl<V, W, true>
                {
                    template<typename X, typename Y>
                    static yes_type test(typename remove_reference
                                         <
                                             decltype( declval<X &>() < declval<Y &>() )
                                         >::type *);

                    template<typename X, typename Y>
                    static no_type test(...);

                    static constexpr bool value = sizeof(test<V, W>(nullptr)) == sizeof(yes_type);
                };

                static constexpr bool value = impl
                                              <
                                                  T,
                                                  U,
                                                  !is_void<T>::value && !is_void<U>::value
                                              >::value;
            };

            template<typename T, typename U, bool can_invoke_op_less_on_t_and_u>
            struct impl_result_of_op_less
            {
            };

            template<typename T, typename U>
            struct impl_result_of_op_less<T, U, true>
            {
                typedef decltype( declval<T &>() < declval<U &>() ) type;
            };

            template<typename T, typename U>
            struct result_of_op_less : impl_result_of_op_less
                                       <
                                           T,
                                           U,
                                           can_invoke_op_less<T, U>::value
                                       >
            {
            };
        }

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// lvalues of types LhsArgument and RhsArgument
        /// can be compared using operator <
        template<typename LhsArgument, typename RhsArgument>
        struct are_comparable_op_less
        {
            template<typename V, typename W, bool can_invoke_op_less_on_v_and_w>
            struct impl
            {
                static constexpr bool value = false;
            };

            template<typename V, typename W>
            struct impl<V, W, true>
            {
                static constexpr bool value = can_be_used_in_boolean_context
                                              <
                                                  typename helpers::result_of_op_less
                                                  <
                                                      V,
                                                      W
                                                  >::type
                                              >::value;
            };

            static constexpr bool value = impl
                                          <
                                              LhsArgument,
                                              RhsArgument,
                                              helpers::can_invoke_op_less
                                              <
                                                  LhsArgument,
                                                  RhsArgument
                                              >::value
                                          >::value;
        };
    }
}

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_LESS_HPP
