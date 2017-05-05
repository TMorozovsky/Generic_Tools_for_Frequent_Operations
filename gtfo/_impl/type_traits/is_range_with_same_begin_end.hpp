#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANGE_WITH_SAME_BEGIN_END_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANGE_WITH_SAME_BEGIN_END_HPP

#include "gtfo/_impl/type_traits/is_range.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// see is_range<>;
        /// this type trait is almost the same
        /// but with an additional restriction:
        /// values returned from
        ///     begin(lvalue-of-type-T)
        /// and
        ///     end(lvalue-of-type-T)
        /// must have the same type
        template<typename T>
        struct is_range_with_same_begin_end
        {
            template<typename U, bool u_is_range>
            struct impl
            {
                static constexpr bool value = false;
            };

            template<typename U>
            struct impl<U, true>
            {
                static constexpr bool value = is_same
                                              <
                                                  typename decay< typename result_of_begin<U>::type >::type,
                                                  typename decay< typename result_of_end<U>::type >::type
                                              >::value;
            };

            static constexpr bool value = impl
                                          <
                                              T,
                                              is_range<T>::value
                                          >::value;
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_IS_RANGE_WITH_SAME_BEGIN_END_HPP

