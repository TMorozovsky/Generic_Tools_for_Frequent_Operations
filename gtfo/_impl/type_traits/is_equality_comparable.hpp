#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_EQUALITY_COMPARABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_EQUALITY_COMPARABLE_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename T, typename U>
            struct can_invoke_comparison_eq
            {
                template<typename V, typename W, bool v_and_w_are_not_void>
                struct impl
                {
                    static GTFO_CONSTEXPR bool value = false;
                };

                template<typename V, typename W>
                struct impl<V, W, true>
                {
                    template<typename X, typename Y>
                    static yes_type test(typename remove_reference
                                         <
                                             decltype( declval<const X &>() == declval<const Y &>() )
                                         >::type *);

                    template<typename X, typename Y>
                    static no_type test(...);

                    static GTFO_CONSTEXPR bool value = sizeof(test<V, W>(nullptr)) == sizeof(yes_type);
                };

                static GTFO_CONSTEXPR bool value = impl
                                                   <
                                                       T,
                                                       U,
                                                       !is_void<T>::value && !is_void<U>::value
                                                   >::value;
            };

            template<typename T, typename U>
            struct can_invoke_comparison_n_eq
            {
                template<typename V, typename W, bool v_and_w_are_not_void>
                struct impl
                {
                    static GTFO_CONSTEXPR bool value = false;
                };

                template<typename V, typename W>
                struct impl<V, W, true>
                {
                    template<typename X, typename Y>
                    static yes_type test(typename remove_reference
                                         <
                                             decltype( declval<const X &>() != declval<const Y &>() )
                                         >::type *);

                    template<typename X, typename Y>
                    static no_type test(...);

                    static GTFO_CONSTEXPR bool value = sizeof(test<V, W>(nullptr)) == sizeof(yes_type);
                };

                static GTFO_CONSTEXPR bool value = impl
                                                   <
                                                       T,
                                                       U,
                                                       !is_void<T>::value && !is_void<U>::value
                                                   >::value;
            };

            template<typename T, typename U, bool can_invoke_comparison_eq_on_t_and_u>
            struct impl_result_of_eq_comparison
            {
            };

            template<typename T, typename U>
            struct impl_result_of_eq_comparison<T, U, true>
            {
                typedef decltype( declval<const T &>() == declval<const U &>() ) type;
            };

            template<typename T, typename U, bool can_invoke_comparison_n_eq_on_t_and_u>
            struct impl_result_of_n_eq_comparison
            {
            };

            template<typename T, typename U>
            struct impl_result_of_n_eq_comparison<T, U, true>
            {
                typedef decltype( declval<const T &>() != declval<const U &>() ) type;
            };

            template<typename T, typename U>
            struct result_of_eq_comparison : impl_result_of_eq_comparison<T, U,
                                                                          can_invoke_comparison_eq<T, U>::value>
            {
            };

            template<typename T, typename U>
            struct result_of_n_eq_comparison : impl_result_of_n_eq_comparison<T, U,
                                                                              can_invoke_comparison_n_eq<T, U>::value>
            {
            };

            // note:
            // in MSVC 2012, there is a bug which answers "true"
            // if T's boolean cast operator is private
            template<typename T>
            struct can_be_used_in_a_boolean_context
            {
                template<typename U>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( static_cast<bool>(declval<T>()) )
                                     >::type *);

                template<typename U>
                static no_type test(...);

                static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
            };
        }

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// types T and U are equality-comparable, i.e. expressions
        ///     const-lvalue-of-type-T == const-lvalue-of-type-U
        /// and
        ///     const-lvalue-of-type-T != const-lvalue-of-type-U
        /// are both well-formed and return something that can be used
        /// in a boolean context
        template<typename T, typename U>
        struct is_equality_comparable
        {
            template<typename V, typename W, bool can_invoke_both_equality_comparisons_on_v_and_w>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename V, typename W>
            struct impl<V, W, true>
            {
                static GTFO_CONSTEXPR bool value = helpers::can_be_used_in_a_boolean_context
                                                   <
                                                       typename helpers::result_of_eq_comparison
                                                       <
                                                           V,
                                                           W
                                                       >::type
                                                   >::value
                                                   &&
                                                   helpers::can_be_used_in_a_boolean_context
                                                   <
                                                       typename helpers::result_of_n_eq_comparison
                                                       <
                                                           V,
                                                           W
                                                       >::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   U,
                                                   helpers::can_invoke_comparison_eq<T, U>::value &&
                                                   helpers::can_invoke_comparison_n_eq<T, U>::value
                                               >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_EQUALITY_COMPARABLE_HPP
