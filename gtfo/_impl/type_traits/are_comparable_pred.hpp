#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_PRED_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_PRED_HPP

#include "gtfo/_impl/type_traits/is_predicate2.hpp"

namespace gtfo
{
    namespace _tt
    {
        /// defines static member constant value of type bool
        /// which is true if and only if
        /// an lvalue of type BinaryPredicate can be used to compare
        /// lvalues of types LhsArgument and RhsArgument
        template<typename BinaryPredicate, typename LhsArgument, typename RhsArgument>
        struct are_comparable_pred : is_predicate2<BinaryPredicate, LhsArgument &, RhsArgument &>
        {
        };
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_ARE_COMPARABLE_OP_EQ_HPP
