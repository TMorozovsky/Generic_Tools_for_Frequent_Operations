#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_GREATER_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_GREATER_HPP

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
        namespace detail
        {
            struct _can_invoke_op_greater_tester
            {
                template<typename T, typename U>
                static yes_type test(typename remove_reference<
                                         decltype( declval<T>() > declval<U>() )
                                     >::type *);

                template<typename T, typename U>
                static no_type test(...);
            };

            template<typename T, typename U>
            struct can_invoke_op_greater
            {

                static constexpr bool value = sizeof(_can_invoke_op_greater_tester::test<T, U>(nullptr)) == sizeof(yes_type);
            };
        }

        namespace detail
        {
            template<typename T, typename U, bool can_invoke_op_greater_on_T_and_U>
            struct _result_of_op_greater_impl
            {
            };

            template<typename T, typename U>
            struct _result_of_op_greater_impl<T, U, true>
            {
                using type = decltype( declval<T &>() > declval<U &>() );
            };

            template<typename T, typename U>
            struct result_of_op_greater : _result_of_op_greater_impl<T, U, can_invoke_op_greater<T, U>::value>
            {
            };
        }

        namespace detail
        {
            template<typename T, typename U, bool can_invoke_op_greater_on_T_and_U>
            struct _are_comparable_op_greater_impl
            {
                static constexpr bool value = false;
            };

            template<typename T, typename U>
            struct _are_comparable_op_greater_impl<T, U, true>
            {
                static constexpr bool value = can_be_used_in_boolean_context<
                                                  typename result_of_op_greater<T, U>::type
                                              >::value;
            };
        }

        // Defines a static member constant value of type bool
        // which is true if values of types T and U
        // can be compared using operator >.
        template<typename T, typename U>
        struct are_comparable_op_greater
        {
            static constexpr bool value = detail::_are_comparable_op_greater_impl< T, U,
                                                                                   detail::can_invoke_op_greater<T, U>::value >::value;
        };
    }
}

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_GREATER_HPP
