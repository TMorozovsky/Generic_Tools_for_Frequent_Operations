#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_HAS_RBEGIN_MEM_FUN_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_HAS_RBEGIN_MEM_FUN_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// a call to
        ///     (lvalue-of-type-T).rbegin()
        /// is well-formed
        template<typename T>
        struct has_rbegin_mem_fun
        {
            template<typename U>
            static yes_type test(typename remove_reference
                                 <
                                     decltype( declval<U &>().rbegin() )
                                 >::type *);

            template<typename U>
            static no_type test(...);

            static GTFO_CONSTEXPR bool value = sizeof(test<T>(nullptr)) == sizeof(yes_type);
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_HAS_RBEGIN_MEM_FUN_HPP
