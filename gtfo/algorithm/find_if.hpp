#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/result_of_fun1.hpp"
#include "gtfo/_impl/type_traits/result_of_range_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    typename _tt::enable_if
    <
        _tt::can_be_used_in_boolean_context
        <
            typename _tt::result_of_fun1
            <
                UnaryPredicate,
                typename _tt::result_of_dereferencing< InputIterator >::type
            >::type
        >::value,
        InputIterator
    >::type
    find_if(InputIterator it_begin,
            InputIterator it_end,
            UnaryPredicate pred)
    {
        return ::std::find_if(::gtfo::move(it_begin),
                              ::gtfo::move(it_end),
                              ::gtfo::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    typename _tt::enable_if
    <
        _tt::can_be_used_in_boolean_context
        <
            typename _tt::result_of_fun1
            <
                UnaryPredicate,
                typename _tt::result_of_range_iterator_dereferencing< Range >::type
            >::type
        >::value,
        typename _tt::iterator_of_range< Range >::type
    >::type
    find_if(Range &&      range,
            UnaryPredicate pred)
    {
        return ::std::find_if(begin(range),
                              end(range),
                              ::gtfo::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_HPP
