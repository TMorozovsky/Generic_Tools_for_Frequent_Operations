#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP

#include <type_traits>
#include <iterator>

namespace gtfo
{
    using ::std::enable_if;
    using ::std::is_same;
    using ::std::is_constructible;
    using ::std::declval;
    using ::std::decay;
    using ::std::iterator_traits;
    using ::std::begin;
    using ::std::end;

    using yes_type = char;
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

        static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
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

        static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
    };

    namespace helpers
    {
        template<typename C, bool c_has_begin>
        struct impl_result_of_begin
        {
        };

        template<typename C>
        struct impl_result_of_begin<C, true>
        {
            using type = typename decay
                                  <
                                      decltype( begin(declval<C &>()) )
                                  >::type;
        };

        template<typename C, bool c_has_end>
        struct impl_result_of_end
        {
        };

        template<typename C>
        struct impl_result_of_end<C, true>
        {
            using type = typename decay
                                  <
                                      decltype( end(declval<C &>()) )
                                  >::type;
        };
    }

    /// declares member type which represents what a call to
    ///     begin(object-of-type-T)
    /// returns
    /// top-level reference and cv-qualifiers (if any) are discarded
    template<typename T>
    struct result_of_begin : helpers::impl_result_of_begin<T, has_begin<T>::value>
    {
    };

    /// declares member type which represents what a call to
    ///     end(object-of-type-T)
    /// returns
    /// top-level reference and cv-qualifiers (if any) are discarded
    template<typename T>
    struct result_of_end : helpers::impl_result_of_end<T, has_end<T>::value>
    {
    };

    /// defines static member constant value of type bool
    /// which is true if and only if
    /// an object of type T is a valid iterator
    template<typename T>
    struct is_iterator
    {
        template<typename U, typename = void>
        struct impl
        {
           static constexpr bool value = false;
        };

        template<typename U>
        struct impl<U, typename enable_if
                       <
                           !is_same
                           <
                               typename iterator_traits<U>::value_type,
                               void
                           >::value
                       >::type
                   >
        {
           static constexpr bool value = true;
        };

        static constexpr bool value = impl<T>::value;
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
            static constexpr bool value = false;
        };

        template<typename U>
        struct impl<U, true>
        {
            static constexpr bool value = is_iterator
                                          <
                                              typename result_of_begin<U>::type
                                          >::value;
        };

        static constexpr bool value = impl
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
            static constexpr bool value = false;
        };

        template<typename U>
        struct impl<U, true>
        {
            static constexpr bool value = is_iterator
                                          <
                                              typename result_of_end<U>::type
                                          >::value;
        };

        static constexpr bool value = impl
                                      <
                                          T,
                                          has_end<T>::value
                                      >::value;
    };

    /// defines static member constant value of type bool
    /// which is true if and only if
    /// both calls to
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
            static constexpr bool value = false;
        };

        template<typename U>
        struct impl<U, true>
        {
            static constexpr bool value = is_same
                                          <
                                              typename result_of_begin<U>::type,
                                              typename result_of_end<U>::type
                                          >::value;
        };

        static constexpr bool value = impl
                                      <
                                          T,
                                          has_iterator_returning_begin<T>::value && has_iterator_returning_end<T>::value
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
            using type = typename decay
                         <
                             decltype( *begin(declval<C &>()) )
                         >::type;
        };
    }

    /// declares member type which tells
    /// elements of what type does a container of type T store
    template<typename T>
    struct element_of_container : helpers::impl_element_of_container<T, is_container<T>::value>
    {
    };

    /// defines static member constant value of type bool
    /// which is true if and only if
    /// Container is type of a container that stores elements
    /// which can be added to an object of type Value
    /// using the specified Adder function
    /// (if Adder is not specified, operator+ is used instead)
    template<typename Container, typename Value, typename Adder = void>
    struct can_add_elements_of_container_to_value
    {
        template<typename C, typename V, typename A, bool c_is_container, bool adder_specified>
        struct impl
        {
            static constexpr bool value = false;
        };

        template<typename C, typename V, typename A>
        struct impl<C, V, A, true, false>
        {
            template<typename Lhs, typename Rhs>
            static yes_type test(decltype(declval<const Lhs &>() + declval<const Rhs &>()) *);

            template<typename Lhs, typename Rhs>
            static no_type test(...);

            using element_type = typename element_of_container<C>::type;
            static constexpr bool value = sizeof(test<V, element_type>(nullptr)) == sizeof(yes_type);
        };

        template<typename C, typename V, typename A>
        struct impl<C, V, A, true, true>
        {
            template<typename Lhs, typename Rhs>
            static yes_type test(decltype(declval<A>()(declval<const Lhs &>(), declval<const Rhs &>())) *);

            template<typename Lhs, typename Rhs>
            static no_type test(...);

            using element_type = typename element_of_container<C>::type;
            static constexpr bool value = sizeof(test<V, element_type>(nullptr)) == sizeof(yes_type);
        };

        static constexpr bool value = impl
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
    /// Container is type of a container and
    /// Function is type of a binary function (or function object)
    /// which can be invoked on two elements from such container
    template<typename Function, typename Container>
    struct invokable_on_2_elements_of_container
    {
        template<typename F, typename C, bool c_is_container>
        struct impl
        {
            static constexpr bool value = false;
        };

        template<typename F, typename C>
        struct impl<F, C, true>
        {
            template<typename El>
            static yes_type test(decltype(declval<F>()(declval<const El &>(), declval<const El &>())) *);

            template<typename El>
            static no_type test(...);

            using element_type = typename element_of_container<C>::type;
            static constexpr bool value = sizeof(test<element_type>(nullptr)) == sizeof(yes_type);
        };

        static constexpr bool value = impl
                                      <
                                          Function,
                                          Container,
                                          is_container<Container>::value
                                      >::value;
    };
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_HPP

