#ifndef GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_predicate1.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_range_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    typename _tt::enable_if
    <
        _tt::is_predicate1
        <
            UnaryPredicate,
            typename _tt::result_of_dereferencing< InputIterator >::type
        >::value,
        bool
    >::type
    all_of(InputIterator  it_begin,
           InputIterator  it_end,
           UnaryPredicate pred)
    {
        return ::std::all_of(::gtfo::move(it_begin),
                             ::gtfo::move(it_end),
                             ::gtfo::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    typename _tt::enable_if
    <
        _tt::is_predicate1
        <
            UnaryPredicate,
            typename _tt::result_of_range_iterator_dereferencing< Range >::type
        >::value,
        bool
    >::type
    all_of(Range &&       range,
           UnaryPredicate pred)
    {
        return ::std::all_of(begin(range),
                             end(range),
                             ::gtfo::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP
