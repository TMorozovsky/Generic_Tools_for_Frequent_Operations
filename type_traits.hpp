#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP

#include <type_traits>
#include <iterator>

#if __cplusplus >= 201103L
#define GTFO_CONSTEXPR constexpr
#else
#define GTFO_CONSTEXPR const
#endif

namespace gtfo
{
    using ::std::begin;
    using ::std::end;

    namespace tt
    {
        using ::std::enable_if;
        using ::std::is_assignable;
        using ::std::is_same;
        using ::std::is_void;
        using ::std::remove_reference;
        using ::std::decay;
        using ::std::declval;

        typedef char yes_type;
        struct no_type { char _[2]; };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     begin(object-of-type-T)
        /// is well-formed
        template<typename T>
        struct has_begin
        {
            template<typename U>
            static yes_type test(decltype(begin(declval<U &>())) *);

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
            static yes_type test(decltype(end(declval<U &>())) *);

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

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// type T is dereferenceable, i.e. expression
        ///     * object-of-type-T
        /// is well-formed
        template<typename T>
        struct is_dereferenceable
        {
            template<typename U>
            static yes_type test(typename decay
                                 <
                                     decltype(* declval<U &>())
                                 >::type *);

            template<typename U>
            static no_type test(...);

    #ifdef _MSC_VER
            template<typename U>
            static no_type test_msvc_workaround(typename enable_if
                                                <
                                                    is_same
                                                    <
                                                        decltype(& * declval<U &>()),
                                                        decltype(&   declval<U &>())
                                                    >::value
                                                >::type *);

            template<typename U>
            static yes_type test_msvc_workaround(...);
    #endif

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type)
    #ifdef _MSC_VER
                                               && sizeof(test_msvc_workaround<T>(nullptr)) == sizeof(yes_type)
    #endif
                    ;
        };

        namespace helpers
        {
            template<typename T, bool t_is_dereferenceable>
            struct impl_declared_dereferencing_result
            {
            };

            template<typename T>
            struct impl_declared_dereferencing_result<T, true>
            {
                typedef decltype(* declval<T &>()) type;
            };

            template<typename T, bool t_is_dereferenceable>
            struct impl_dereferencing_result
            {
            };

            template<typename T>
            struct impl_dereferencing_result<T, true>
            {
                typedef typename decay
                        <
                            decltype(* declval<T &>())
                        >::type type;
            };
        }

        /// declares member type which is the type of expression
        ///     * object-of-type-T
        /// stored "as is", without stripping any top-level reference
        /// or cv-qualifiers;
        /// if is_dereferenceable<T>::value == false, no member type is provided
        template<typename T>
        struct declared_dereferencing_result : helpers::impl_declared_dereferencing_result<T, is_dereferenceable<T>::value>
        {
        };

        /// declares member type which is the type of expression
        ///     * object-of-type-T
        /// with top-level reference and cv-qualifiers stripped;
        /// if is_dereferenceable<T>::value == false, no member type is provided
        template<typename T>
        struct dereferencing_result : helpers::impl_dereferencing_result<T, is_dereferenceable<T>::value>
        {
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// type T is incrementable, i.e. expression
        ///     ++ object-of-type-T
        /// is well-formed
        template<typename T>
        struct is_incrementable
        {
            template<typename U>
            static yes_type test(typename decay< decltype(++ declval<U &>()) >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// object of type T is a valid iterator that
        /// can be (at least) dereferenced and incremented
        template<typename T>
        struct is_iterator
        {
            template<typename U, bool u_is_incrementable_and_dereferenceable>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static GTFO_CONSTEXPR bool value = !is_void
                                                   <
                                                       typename dereferencing_result<U>::type
                                                   >::value;
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   T,
                                                   is_incrementable<T>::value &&
                                                   is_dereferenceable<T>::value
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
            struct impl_element_of_container
            {
            };

            template<typename C>
            struct impl_element_of_container<C, true>
            {
                typedef typename decay
                        <
                            decltype( *begin(declval<C &>()) )
                        >::type type;
            };
        }

        /// declares member type which tells
        /// elements of what type does a container of type T store;
        /// if is_container<T>::value == false, no member type is provided
        template<typename T>
        struct element_of_container : helpers::impl_element_of_container<T, is_container<T>::value>
        {
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// Container is a valid type of container that stores
        /// elements which can be added to an object of type Value
        /// using a function object of type Adder;
        /// an Adder should accept two arguments: a Value as its first argument
        /// and a container element as its second argument;
        /// if Adder type is not specified, the expression
        ///     object-of-type-Value + object-of-Container-element-type
        /// is used instead
        template<typename Container, typename Value, typename Adder = void>
        struct can_add_elements_of_container_to_value
        {
            template<typename C, typename V, typename A, bool c_is_container, bool adder_specified>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename C, typename V, typename A>
            struct impl<C, V, A, true, false>
            {
                template<typename Lhs, typename Rhs>
                static yes_type test(decltype(declval<const Lhs &>() + declval<const Rhs &>()) *);

                template<typename Lhs, typename Rhs>
                static no_type test(...);

                typedef typename element_of_container<C>::type element_type;
                static GTFO_CONSTEXPR bool value = sizeof(test<V, element_type>(nullptr)) == sizeof(yes_type);
            };

            template<typename C, typename V, typename A>
            struct impl<C, V, A, true, true>
            {
                template<typename Lhs, typename Rhs>
                static yes_type test(decltype(declval<A &>()(declval<const Lhs &>(), declval<const Rhs &>())) *);

                template<typename Lhs, typename Rhs>
                static no_type test(...);

                typedef typename element_of_container<C>::type element_type;
                static GTFO_CONSTEXPR bool value = sizeof(test<V, element_type>(nullptr)) == sizeof(yes_type);
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   Container,
                                                   Value,
                                                   Adder,
                                                   is_container<Container>::value,
                                                   !is_same<Adder, void>::value
                                               >::value;
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// Container is a valid type of container and
        /// BinaryOperation is a type of binary function or function object
        /// which can be invoked on two elements from such container
        template<typename BinaryOperation, typename Container>
        struct can_invoke_operation_on_two_container_elements
        {
            template<typename O, typename C, bool c_is_container>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename O, typename C>
            struct impl<O, C, true>
            {
                template<typename E>
                static yes_type test(decltype(declval<O &>()(declval<const E &>(), declval<const E &>())) *);

                template<typename E>
                static no_type test(...);

                typedef typename element_of_container<C>::type element_type;
                static GTFO_CONSTEXPR bool value = sizeof(test<element_type>(nullptr)) == sizeof(yes_type);
            };

            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   BinaryOperation,
                                                   Container,
                                                   is_container<Container>::value
                                               >::value;
        };

        /// defines static member constant value of type bool
        /// which is true if and only if
        /// Container is a valid type of container
        /// that stores elements which can be assigned to
        ///     * object-of-type-OutputIterator
        template<typename Container, typename OutputIterator>
        struct can_assign_container_element_to_dereferenced_output_iterator
        {
            template<typename C, typename I, bool c_is_container_and_i_is_iterator>
            struct impl
            {
                static GTFO_CONSTEXPR bool value = false;
            };

            template<typename C, typename I>
            struct impl<C, I, true>
            {
                typedef typename element_of_container<C>::type element_type;
                typedef typename declared_dereferencing_result<I>::type dest_type;
                static GTFO_CONSTEXPR bool value = is_assignable<dest_type, element_type>::value;
            };


            static GTFO_CONSTEXPR bool value = impl
                                               <
                                                   Container,
                                                   OutputIterator,
                                                   is_container<Container>::value &&
                                                   is_iterator<OutputIterator>::value
                                               >::value;
        };
    }
}

#undef GTFO_CONSTEXPR

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP
