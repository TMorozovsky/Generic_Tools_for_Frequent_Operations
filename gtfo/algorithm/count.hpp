#ifndef GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
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
            const Value &
        >::value,
        decltype(::std::count(_tt::declval< InputIterator >(),
                              _tt::declval< InputIterator >(),
                              _tt::declval< const Value & >()))
    >::type
    count(InputIterator it_begin,
          InputIterator it_end,
          const Value & value)
    {
        return ::std::count(::gtfo::move(it_begin),
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
            const Value &
        >::value,
        decltype(::std::count(_tt::declval< typename _tt::iterator_of_range< Range >::type >(),
                              _tt::declval< typename _tt::iterator_of_range< Range >::type >(),
                              _tt::declval< const Value & >()))
    >::type
    count(Range &&      range,
          const Value & value)
    {
        return ::std::count(begin(range),
                            end(range),
                            value);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP
