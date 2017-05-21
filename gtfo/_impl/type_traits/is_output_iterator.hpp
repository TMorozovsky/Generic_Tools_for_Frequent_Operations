#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_OUTPUT_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_OUTPUT_ITERATOR_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/is_incrementable.hpp"
#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template< typename ResultOfDereferencing,
                      bool ResultOfDereferencing_is_reference,
                      bool remove_ref_ResultOfDereferencing_is_arithmetic_or_enum_or_pointer,
                      bool remove_ref_ResultOfDereferencing_is_class_or_union >
            struct _is_output_iterator_impl2;

            template<typename R>
            struct _is_output_iterator_impl2<R, false, false, false> // R is not a reference, not arithmetic/enum/pointer, not a class/struct/union
            {
                static constexpr bool value = false; // was that void or nullptr_t?.. whatever, we cannot assign to it
            };

            template<typename R>
            struct _is_output_iterator_impl2<R, false, true, false> // R is a value of arithmetic type, enum or pointer
            {
                static constexpr bool value = false; // we cannot assign to rvalues of such types
            };

            template<typename R>
            struct _is_output_iterator_impl2<R, false, false, true> // R is rvalue object of some user-defined type
            {
                static constexpr bool value = true; // it could be some proxy with an overloaded assignment operator
            };

            template<typename R, bool unused0, bool unused1>
            struct _is_output_iterator_impl2<R, true, unused0, unused1> // R is a reference type
            {
                static constexpr bool value = !is_const<remove_reference_t<R>>::value; // can assign if this is not a reference to const
            };

            template<typename T, bool T_satisfies_basic_requirements>
            struct is_output_iterator_impl
            {
                static constexpr bool value = false;
            };

            template<typename T>
            struct is_output_iterator_impl<T, true>
            {
                using R = result_of_dereferencing_t<T>;

                static constexpr bool value = _is_output_iterator_impl2< R,
                                                                         is_reference<R>::value,
                                                                         is_arithmetic<R>::value || is_enum<R>::value || is_pointer<R>::value,
                                                                         is_class<R>::value || is_union<R>::value >::value;
            };
        }

        // Defines a static member constant value of type bool
        // which is true if type T can be treated as a valid output iterator.
        template<typename T>
        struct is_output_iterator
        {
            static constexpr bool value = detail::is_output_iterator_impl< T,
                                                                           is_copy_constructible<T>::value &&
                                                                           is_copy_assignable<T>::value &&
                                                                           is_destructible<T>::value &&
                                                                           is_incrementable<T>::value &&
                                                                           is_dereferenceable<T>::value >::value;
        };

        // Pointers are iterators, but pointers to const cannot be used as output iterators
        template<typename T>
        struct is_output_iterator<const T *>
        {
            static constexpr bool value = false;
        };

        // References are not iterators.
        template<typename T>
        struct is_output_iterator<T &>
        {
            static constexpr bool value = false;
        };
        template<typename T>
        struct is_output_iterator<T &&>
        {
            static constexpr bool value = false;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_OUTPUT_ITERATOR_HPP
