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
    for_each(InputIterator it_begin,
             InputIterator it_end,
             UnaryFunction fun)
    {
        return ::std::for_each(_utils::move(it_begin),
                               _utils::move(it_end),
                               _utils::move(fun));
    }

    template<typename Range, typename UnaryFunction>
    inline
    UnaryFunction
    for_each(Range &&      range,
             UnaryFunction fun)
    {
        return ::std::for_each(begin(range),
                               end(range),
                               _utils::move(fun));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
