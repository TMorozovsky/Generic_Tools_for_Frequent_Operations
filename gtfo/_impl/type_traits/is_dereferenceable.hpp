#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
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

                    static constexpr bool value = sizeof(test_value_type       <T>(nullptr)) == sizeof(yes_type) &&
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
        ///     * lvalue-of-type-T
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
                                                >::type *);

            template<typename U>
            static yes_type test_msvc_workaround(...);
    #endif

            static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type)
    #ifdef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
                                          && !is_fundamental< typename remove_reference<T>::type >::value
                                          && (
                                                 sizeof(test_msvc_workaround<T>(nullptr)) == sizeof(yes_type)
                                                 ||
                                                 helpers::msvc_specific_workarounds::has_some_nested_iterator_typedefs
                                                 <
                                                     typename remove_reference<T>::type
                                                 >::value
                                             )
    #endif
                    ;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_DEREFERENCEABLE_HPP
