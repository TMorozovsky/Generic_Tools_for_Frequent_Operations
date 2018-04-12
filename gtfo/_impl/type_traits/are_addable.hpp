#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_ADDABLE_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_ADDABLE_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/type_traits/void_t.hpp"

namespace gtfo
{
    namespace _tt
    {
        namespace detail
        {
            template<typename Lhs, typename Rhs, typename = void_t<>>
            struct are_addable
                : false_type
            {
            };

            template<typename Lhs, typename Rhs>
            struct are_addable<Lhs, Rhs, void_t<decltype( declval<Lhs>() + declval<Rhs>() )>>
                : true_type
            {
            };
        }

        // Defines a static member constant value of type bool
        // which is true if the expression
        //     lvalue-of-type-Lhs + lvalue-of-type-Rhs
        // is well-formed.
        template<typename Lhs, typename Rhs>
        struct are_addable
            : detail::are_addable<add_lvalue_reference_t<Lhs>, add_lvalue_reference_t<Rhs>>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_ADDABLE_HPP
