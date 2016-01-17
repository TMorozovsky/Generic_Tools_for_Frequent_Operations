#ifndef GTFO_FILE_INCLUDED_ALGORITHM_ADJACENT_FIND_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_ADJACENT_FIND_HPP

/*
 * Defines the following overloads:
 *     adjacent_find(ForwardIterator, ForwardIterator);
 *     adjacent_find(ForwardIterator, ForwardIterator, BinaryPredicate);
 *     adjacent_find(Range);
 *     adjacent_find(Range,           BinaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_ADJACENT_FIND(ForwardIterator) \
    typename _tt::enable_if                                  \
    <                                                              \
        _tt::are_comparable_op_eq                                         \
        <                                                                    \
            typename _tt::result_of_dereferencing< ForwardIterator >::type,   \
            typename _tt::result_of_dereferencing< ForwardIterator >::type    \
        >::value,                                                           \
        ForwardIterator                                                 \
    >::type                                                      \

#define GTFO_RESULT_OF_ADJACENT_FIND_PRED(ForwardIterator, BinaryPredicate) \
    typename _tt::enable_if                                                 \
    <                                                                       \
        _tt::are_comparable_pred                                            \
        <                                                                   \
            BinaryPredicate,                                                \
            typename _tt::result_of_dereferencing< ForwardIterator >::type, \
            typename _tt::result_of_dereferencing< ForwardIterator >::type  \
        >::value,                                                          \
        ForwardIterator                                                  \
    >::type                                                           \

    template<typename ForwardIterator>
    inline
    GTFO_RESULT_OF_ADJACENT_FIND(ForwardIterator)
    adjacent_find(ForwardIterator it_begin,
                  ForwardIterator it_end)
    {
        return ::std::adjacent_find(_utils::move(it_begin),
                                    _utils::move(it_end));
    }

    template<typename ForwardIterator, typename BinaryPredicate>
    inline
    ForwardIterator
    adjacent_find(ForwardIterator it_begin,
                  ForwardIterator it_end,
                  BinaryPredicate pred)
    {
        return ::std::adjacent_find(_utils::move(it_begin),
                                    _utils::move(it_end),
                                    _utils::move(pred));
    }

    template<typename Range>
    inline
    typename _tt::iterator_of_range< Range >::type
    adjacent_find(Range && range)
    {
        return ::std::adjacent_find(begin(range),
                                    end(range));
    }

    template<typename Range, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_ADJACENT_FIND_PRED(typename _tt::iterator_of_range< Range >::type,
                                      BinaryPredicate)
    adjacent_find(Range &&        range,
                  BinaryPredicate pred)
    {
        return ::std::adjacent_find(begin(range),
                                    end(range),
                                    _utils::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_ADJACENT_FIND_HPP
