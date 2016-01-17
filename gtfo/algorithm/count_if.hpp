#ifndef GTFO_FILE_INCLUDED_ALGORITHM_COUNT_IF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_COUNT_IF_HPP

/*
 * Defines the following overloads:
 *     count_if(InputIterator, InputIterator,  UnaryPredicate);
 *     count_if(Range,         UnaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/result_of_begin.hpp"
#include "gtfo/_impl/type_traits/result_of_end.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    auto
    count_if(InputIterator  it_begin,
             InputIterator  it_end,
             UnaryPredicate pred) -> decltype(::std::count_if(_tt::declval< InputIterator >(),
                                                              _tt::declval< InputIterator >(),
                                                              _tt::declval< UnaryPredicate >()))
    {
        return ::std::count_if(_utils::move(it_begin),
                               _utils::move(it_end),
                               _utils::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    auto
    count_if(Range &&       range,
             UnaryPredicate pred) -> decltype(::std::count_if(_tt::declval< typename _tt::result_of_begin< Range >::type >(),
                                                              _tt::declval< typename _tt::result_of_end< Range >::type >(),
                                                              _tt::declval< UnaryPredicate >()))
    {
        return ::std::count_if(begin(range),
                               end(range),
                               _utils::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_COUNT_IF_HPP
