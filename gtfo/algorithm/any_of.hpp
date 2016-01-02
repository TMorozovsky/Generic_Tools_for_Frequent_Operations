#ifndef GTFO_FILE_INCLUDED_ALGORITHM_ANY_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_ANY_OF_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"
#include "gtfo/_impl/type_traits/result_of_fun1.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
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
        bool
    >::type
    any_of(InputIterator  it_begin,
           InputIterator  it_end,
           UnaryPredicate pred)
    {
        return ::std::any_of(::gtfo::move(it_begin),
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
        bool
    >::type
    any_of(Range &&       range,
           UnaryPredicate pred)
    {
        return ::std::any_of(begin(range),
                             end(range),
                             ::gtfo::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_ANY_OF_HPP
