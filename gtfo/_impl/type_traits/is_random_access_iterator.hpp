#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANDOM_ACCESS_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANDOM_ACCESS_ITERATOR_HPP

#include "gtfo/_impl/type_traits/is_bidirectional_iterator.hpp"
#include "gtfo/_impl/type_traits/are_addable.hpp"
#include "gtfo/_impl/type_traits/are_subtractable.hpp"
#include "gtfo/_impl/type_traits/is_compound_addable.hpp"
#include "gtfo/_impl/type_traits/is_compound_subtractable.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_less.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_less_or_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_greater.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_greater_or_eq.hpp"
#include "gtfo/_impl/type_traits/can_apply_array_subscript.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<typename I>
            struct satisfies_additional_random_access_iterator_requirements
            {
                static constexpr bool value = are_addable                    < I, ptrdiff_t >::value &&
                                              are_addable                    < ptrdiff_t, I >::value &&
                                              are_subtractable               < I, ptrdiff_t >::value &&
                                              are_subtractable               < I, I         >::value &&
                                              is_compound_addable            < I, ptrdiff_t >::value &&
                                              is_compound_subtractable       < I, ptrdiff_t >::value &&
                                              are_comparable_op_less         < I, I         >::value &&
                                              are_comparable_op_less_or_eq   < I, I         >::value &&
                                              are_comparable_op_greater      < I, I         >::value &&
                                              are_comparable_op_greater_or_eq< I, I         >::value &&
                                              can_apply_array_subscript      < I, ptrdiff_t >::value;
            };
        }

        // Defines a static member constant value of type bool which is true
        // if type T can be treated as a valid random-access iterator.
        template<typename T>
        struct is_random_access_iterator
        {
            static constexpr bool value = is_bidirectional_iterator<T>::value &&
                                          detail::satisfies_additional_random_access_iterator_requirements<T>::value;
        };

        // Defines a static member constant value of type bool which is true
        // if type T can be treated as a valid random-access iterator
        // that allows modification of its dereferencing result.
        template<typename T>
        struct is_mutable_random_access_iterator
        {
            static constexpr bool value = is_mutable_bidirectional_iterator<T>::value &&
                                          detail::satisfies_additional_random_access_iterator_requirements<T>::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANDOM_ACCESS_ITERATOR_HPP
