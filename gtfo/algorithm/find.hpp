#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/result_of_range_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator, typename Value>
    inline
    typename _tt::enable_if
    <
        _tt::are_comparable_op_eq
        <
            typename _tt::result_of_dereferencing< InputIterator >::type,
            Value
        >::value,
        InputIterator
    >::type
    find(InputIterator it_begin,
         InputIterator it_end,
         const Value & value)
    {
        return ::std::find(::gtfo::move(it_begin),
                           ::gtfo::move(it_end),
                           value);
    }

    template<typename Range, typename Value>
    inline
    typename _tt::enable_if
    <
        _tt::are_comparable_op_eq
        <
            typename _tt::result_of_range_iterator_dereferencing< Range >::type,
            Value
        >::value,
        typename _tt::iterator_of_range< Range >::type
    >::type
    find(Range &&      range,
         const Value & value)
    {
        return ::std::find(begin(range),
                           end(range),
                           value);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_HPP
