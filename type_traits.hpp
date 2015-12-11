#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP

#include <type_traits>
#include <iterator>

#if (__cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define GTFO_CONSTEXPR constexpr
#else
#define GTFO_CONSTEXPR const
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#define GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
#endif

namespace gtfo
{
    using ::std::begin;
    using ::std::end;

    namespace tt
    {
        using ::std::enable_if;
        using ::std::is_same;
        using ::std::is_void;
        using ::std::is_const;
        using ::std::remove_reference;
        using ::std::decay;
        using ::std::declval;

        typedef char(&yes_type)[1];
        typedef char(&no_type) [2];

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     begin(object-of-type-T)
        /// is well-formed
        template<typename T>
        struct has_begin
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( begin(declval<U &>()) )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     end(object-of-type-T)
        /// is well-formed
        template<typename T>
        struct has_end
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( end(declval<U &>()) )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };

        namespace helpers
        {
            template<typename T, bool t_has_begin>
            struct impl_result_of_begin
            {
            };

            template<typename T>
            struct impl_result_of_begin<T, true>
            {
                typedef typename decay
                        <
                            decltype( begin(declval<T &>()) )
                        >::type type;
            };

            template<typename T, bool t_has_end>
            struct impl_result_of_end
            {
            };

            template<typename T>
            struct impl_result_of_end<T, true>
            {
                typedef typename decay
                        <
                            decltype( end(declval<T &>()) )
                        >::type type;
            };
        }

        /// declares member type which is the type returned from
        ///     begin(object-of-type-T);
        /// top-level reference and cv-qualifiers (if any) are stripped;
        /// if has_begin<T>::value == false, no member type is provided
        template<typename T>
        struct result_of_begin : helpers::impl_result_of_begin<T, has_begin<T>::value>
        {
        };

        /// declares member type which is the type returned from
        ///     end(object-of-type-T);
        /// top-level reference and cv-qualifiers (if any) are stripped;
        /// if has_end<T>::value == false, no member type is provided
        template<typename T>
        struct result_of_end : helpers::impl_result_of_end<T, has_end<T>::value>
        {
        };

#ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
        namespace helpers
        {
            namespace msvc_specific_workarounds
            {
                /// defines static member constant value of type bool
                /// which is true if and only if
                /// type T has certain iterator-specific nested typedefs
                template<typename T>
                struct has_some_nested_iterator_typedefs
                {
                    template<typename U>
                    static yes_type test_value_type(typename remove_reference< typename U::value_type >::type *);
                    template<typename U>
                    static no_type test_value_type(...);

                    template<typename U>
                    static yes_type test_difference_type(typename remove_reference< typename U::difference_type >::type *);
                    template<typename U>
                    static no_type test_difference_type(...);

                    template<typename U>
                    static yes_type test_pointer(typename remove_reference< typename U::pointer >::type *);
                    template<typename U>
                    static no_type test_pointer(...);

                    template<typename U>
                    static yes_type test_reference(typename remove_reference< typename U::reference >::type *);
                    template<typename U>
                    static no_type test_reference(...);

                    template<typename U>
                    static yes_type test_iterator_category(typename remove_reference< typename U::iterator_category >::type *);
                    template<typename U>
                    static no_type test_iterator_category(...);

                    static GTFO_CONSTEXPR bool value = sizeof(test_value_type       <T>(nullptr)) == sizeof(yes_type) &&
                                                       sizeof(test_difference_type  <T>(nullptr)) == sizeof(yes_type) &&
                                                       sizeof(test_pointer          <T>(nullptr)) == sizeof(yes_type) &&
                                                       sizeof(test_reference        <T>(nullptr)) == sizeof(yes_type) &&
                                                       sizeof(test_iterator_category<T>(nullptr)) == sizeof(yes_type);
                };
            }
        }
#endif

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// type T is dereferenceable, i.e. expression
        ///     * object-of-type-T
        /// is well-formed
        template<typename T>
        struct is_dereferenceable
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( * declval<U &>() )
                                 >::type *);

            template<typename U>
            static no_type test(...);

    #ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
            template<typename U>
            static no_type test_msvc_workaround(typename enable_if
                                                <
                                                    is_same
                                                    <
                                                        decltype(& * declval<U &>()),
                                                        decltype(&   declval<U &>())
                                                    >::value
                                                    &&
                                                    !helpers::msvc_specific_workarounds::has_some_nested_iterator_typedefs
                                                    <
                                                        U
                                                    >::value
                                                >::type *);

            template<typename U>
            static yes_type test_msvc_workaround(...);
    #endif

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type)
    #ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
                                               && sizeof(test_msvc_workaround<T>(nullptr)) == sizeof(yes_type)
    #endif
                    ;
        };

        namespace helpers
        {
            template<typename T, bool t_is_dereferenceable>
            struct impl_result_of_dereferencing
            {
            };

            template<typename T>
            struct impl_result_of_dereferencing<T, true>
            {
                typedef decltype( * declval<T &>() ) type;
            };

            template<typename T, bool t_is_dereferenceable>
            struct impl_value_of_dereferenced
            {
            };

            template<typename T>
            struct impl_value_of_dereferenced<T, true>
            {
                typedef typename decay
                        <
                            decltype( * declval<T &>() )
                        >::type type;
            };
        }

        /// declares member type which is the type of expression
        ///     * object-of-type-T
        /// stored "as is", without stripping any top-level reference
        /// or cv-qualifiers;
        /// if is_dereferenceable<T>::value == false, no member type is provided
        template<typename T>
        struct result_of_dereferencing : helpers::impl_result_of_dereferencing<T, is_dereferenceable<T>::value>
        {
        };

        /// declares member type which is the type of expression
        ///     * object-of-type-T
        /// with top-level reference and cv-qualifiers stripped
        /// (as if it was returned or passed by value);
        /// if is_dereferenceable<T>::value == false, no member type is provided
        template<typename T>
        struct value_of_dereferenced : helpers::impl_value_of_dereferenced<T, is_dereferenceable<T>::value>
        {
        };

        namespace helpers
        {
            template<typename T, typename U>
            struct can_invoke_comparison_eq
            {
                template<typename V, typename W>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( declval<const V &>() == declval<const W &>() )
                                     >::type *);

                template<typename V, typename W>
                static no_type test(...);

                static GTFO_CONSTEXPR bool value = sizeof(test<T, U>(nullptr)) == sizeof(yes_type);
            };

            template<typename T, typename U>
            struct can_invoke_comparison_n_eq
            {
                template<typename V, typename W>
                static yes_type test(typename remove_reference
                                     <
                                         decltype( declval<const V &>() != declval<const W &>() )
                                     >::type *);

                template<typename V, typename W>
                static no_type test(...);

                static GTFO_CONSTEXPR bool value = sizeof(test<T, U>(nullptr)) == sizeof(yes_type);
            };

            template<typename T, typename U>
            struct can_invoke_both_equality_comparisons
            {
                static GTFO_CONSTEXPR bool value = can_invoke_comparison_eq<T, U>::value &&
                                                   can_invoke_comparison_n_eq<T, U>::value;
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
                                                   helpers::can_invoke_both_equality_comparisons<T, U>::value
                                               >::value;
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// type T is incrementable, i.e. expression
        ///     ++ lvalue-of-type-T
        /// is well-formed
        template<typename T>
        struct is_incrementable
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( ++ declval<U &>() )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// object of type T is a valid iterator that
        /// can be (at least) dereferenced, incremented,
        /// equality-compared to another object of this type,
        /// and its dereferencing result is not void
        template<typename T>
        struct is_iterator
        {
            template<typename U, bool u_is_incrementable_dereferenceable_and_comparable>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = !is_void
                                                   <
                                                       typename value_of_dereferenced<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   is_incrementable<T>::value &&
                                                   is_dereferenceable<T>::value &&
                                                   is_equality_comparable<T, T>::value
                                               >::value;
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     begin(object-of-type-T)
        /// returns a valid iterator
        template<typename T>
        struct has_iterator_returning_begin
        {
            template<typename U, bool u_has_begin>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = is_iterator
                                                   <
                                                       typename result_of_begin<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   has_begin<T>::value
                                               >::value;
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     end(object-of-type-T)
        /// returns a valid iterator
        template<typename T>
        struct has_iterator_returning_end
        {
            template<typename U, bool u_has_end>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = is_iterator
                                                   <
                                                       typename result_of_end<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   has_end<T>::value
                                               >::value;
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// object of type T is a valid container,
        /// i.e. both calls to
        ///     begin(object-of-type-T)
        /// and
        ///     end(object-of-type-T)
        /// return valid iterators of the same type
        template<typename T>
        struct is_container
        {
            template<typename U, bool u_has_iterator_returning_begin_and_end>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = is_same
                                                   <
                                                       typename result_of_begin<U>::type,
                                                       typename result_of_end<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   has_iterator_returning_begin<T>::value &&
                                                   has_iterator_returning_end<T>::value
                                               >::value;
        };

        namespace helpers
        {
            template<typename C, bool c_is_container>
            struct impl_iterator_of_container
            {
            };

            template<typename C>
            struct impl_iterator_of_container<C, true>
            {
                typedef typename decay
                        <
                            decltype( begin(declval<C &>()) )
                        >::type type;
            };

            template<typename C, bool c_is_container>
            struct impl_value_from_container
            {
            };

            template<typename C>
            struct impl_value_from_container<C, true>
            {
                typedef typename decay
                        <
                            decltype( * begin(declval<C &>()) )
                        >::type type;
            };
        }

        /// declares member type which tells
        /// iterators of what type does a container of type T use;
        /// if is_container<T>::value == false, no member type is provided
        template<typename T>
        struct iterator_of_container : helpers::impl_iterator_of_container<T, is_container<T>::value>
        {
        };

        /// declares member type which tells
        /// values of what type does a container of type T store;
        /// if is_container<T>::value == false, no member type is provided
        template<typename T>
        struct value_from_container : helpers::impl_value_from_container<T, is_container<T>::value>
        {
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// Function is a type of function or function object
        /// that can be invoked without any arguments
        template<typename Function>
        struct is_invokable_0
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

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a function or function object of type UnaryOperation
        /// can be invoked with an rvalue of type Argument;
        /// for testing it against an lvalue, add
        /// an lvalue reference to the type of Argument
        template<typename UnaryOperation, typename Argument>
        struct is_invokable_1
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

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a function or function object of type BinaryOperation
        /// can be invoked with two rvalues of type
        /// LhsArgument and RhsArgument
        template<typename BinaryOperation, typename LhsArgument, typename RhsArgument>
        struct is_invokable_2
        {
            template<typename Fun, typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Fun &>()(declval<Lhs>(), declval<Rhs>()) )
                                 >::type *);

            template<typename Fun, typename Lhs, typename Rhs>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test
                                                      <
                                                          BinaryOperation,
                                                          LhsArgument,
                                                          RhsArgument
                                                      >(nullptr)) == sizeof(yes_type);
        };

        /// defines static member constant value of type bool
        /// which is true if and only if an expression
        ///     rvalue-of-type-LhsArgument + rvalue-of-type-RhsArgument
        /// is well-formed
        template<typename LhsArgument, typename RhsArgument>
        struct are_addable
        {
            template<typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Lhs>() + declval<Rhs>() )
                                 >::type *);

            template< typename Lhs, typename Rhs>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test
                                                      <
                                                          LhsArgument,
                                                          RhsArgument
                                                      >(nullptr)) == sizeof(yes_type);
        };

        /// defines static member constant value of type bool
        /// which is true if and only if an expression
        ///     rvalue-of-type-LhsArgument - rvalue-of-type-RhsArgument
        /// is well-formed
        template<typename LhsArgument, typename RhsArgument>
        struct are_subtractable
        {
            template<typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Lhs>() - declval<Rhs>() )
                                 >::type *);

            template< typename Lhs, typename Rhs>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test
                                                      <
                                                          LhsArgument,
                                                          RhsArgument
                                                      >(nullptr)) == sizeof(yes_type);
        };

        namespace helpers
        {
            template<typename Fun, typename Lhs, typename Rhs, bool fun_is_invokable>
            struct impl_result_of_2
            {
            };

            template<typename Fun, typename Lhs, typename Rhs>
            struct impl_result_of_2<Fun, Lhs, Rhs, true>
            {
                typedef decltype( declval<Fun &>()(declval<Lhs>(), declval<Rhs>()) ) type;
            };

            template<typename Lhs, typename Rhs, bool lhs_and_rhs_are_addable>
            struct impl_result_of_addition
            {
            };

            template<typename Lhs, typename Rhs>
            struct impl_result_of_addition<Lhs, Rhs, true>
            {
                typedef decltype( declval<Lhs>() + declval<Rhs>() ) type;
            };

            template<typename Lhs, typename Rhs, bool lhs_and_rhs_are_subtractable>
            struct impl_result_of_subtraction
            {
            };

            template<typename Lhs, typename Rhs>
            struct impl_result_of_subtraction<Lhs, Rhs, true>
            {
                typedef decltype( declval<Lhs>() - declval<Rhs>() ) type;
            };
        }

        /// declares member type which is the type returned by
        /// a call to BinaryOperation function (function object)
        /// with two arguments of types LhsArgument and RhsArgument;
        /// if such call is ill-formed, no member type is provided
        template<typename BinaryOperation, typename LhsArgument, typename RhsArgument>
        struct result_of_2 : helpers::impl_result_of_2<BinaryOperation, LhsArgument, RhsArgument,
                                                       is_invokable_2<BinaryOperation, LhsArgument, RhsArgument>::value>
        {
        };

        /// declares member type which is the type of expression
        ///     rvalue-of-type-LhsArgument + rvalue-of-type-RhsArgument;
        /// for testing it against an lvalues, don't forget to add
        /// lvalue references to argument types;
        /// if such expression is ill-formed, no member type is provided
        template<typename LhsArgument, typename RhsArgument>
        struct result_of_addition : helpers::impl_result_of_addition<LhsArgument, RhsArgument,
                                                                     are_addable<LhsArgument, RhsArgument>::value>
        {
        };

        /// declares member type which is the type of expression
        ///     rvalue-of-type-LhsArgument - rvalue-of-type-RhsArgument;
        /// for testing it against an lvalues, don't forget to add
        /// lvalue references to argument types;
        /// if such expression is ill-formed, no member type is provided
        template<typename LhsArgument, typename RhsArgument>
        struct result_of_subtraction : helpers::impl_result_of_subtraction<LhsArgument, RhsArgument,
                                                                           are_subtractable<LhsArgument, RhsArgument>::value>
        {
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an expression of type From can be assigned
        /// to an expression of type To
        template<typename To, typename From>
        struct is_assignable
        {

            template<typename Lhs, typename Rhs>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<Lhs>() = declval<Rhs>() )
                                 >::type *);

            template<typename Lhs, typename Rhs>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<To, From>(nullptr)) == sizeof(yes_type)
#ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
                    && !is_const<typename remove_reference<To>::type>::value
#endif
                    ;
        };

        /// declares member type which is exactly T;
        /// can be used to disable argument-dependent lookup
        /// when used with overloaded function arguments
        template<typename T>
        struct type_of
        {
            typedef T type;
        };
    }
}

#undef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
#undef GTFO_CONSTEXPR

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP
