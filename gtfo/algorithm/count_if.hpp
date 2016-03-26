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
    count_if(InputIterator  _it_begin,
             InputIterator  _it_end,
             UnaryPredicate _pred) -> decltype(::std::count_if(_tt::declval< InputIterator >(),
                                                               _tt::declval< InputIterator >(),
                                                               _tt::declval< UnaryPredicate >()))
    {
        return ::std::count_if(_utils::move(_it_begin),
                               _utils::move(_it_end),
                               _utils::move(_pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    auto
    count_if(Range &&       _range,
             UnaryPredicate _pred) -> decltype(::std::count_if(_tt::declval< typename _tt::result_of_begin< Range >::type >(),
                                                               _tt::declval< typename _tt::result_of_end< Range >::type >(),
                                                               _tt::declval< UnaryPredicate >()))
    {
        return ::std::count_if(begin(_range),
                               end(_range),
                               _utils::move(_pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_COUNT_IF_HPP
