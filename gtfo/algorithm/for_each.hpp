#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP

/*
 * Defines the following overloads:
 *     for_each(InputIterator, InputIterator, UnaryFunction);
 *     for_each(Range,         UnaryFunction);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryFunction>
    inline
    UnaryFunction
    for_each(InputIterator _it_begin,
             InputIterator _it_end,
             UnaryFunction _fun)
    {
        return ::std::for_each(::gtfo::move(_it_begin),
                               ::gtfo::move(_it_end),
                               ::gtfo::move(_fun));
    }

    template<typename Range, typename UnaryFunction>
    inline
    UnaryFunction
    for_each(Range &&      _range,
             UnaryFunction _fun)
    {
        return ::std::for_each(begin(_range),
                               end(_range),
                               ::gtfo::move(_fun));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
