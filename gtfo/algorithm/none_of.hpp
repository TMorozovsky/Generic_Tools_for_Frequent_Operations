#ifndef GTFO_FILE_INCLUDED_ALGORITHM_NONE_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_NONE_OF_HPP

/*
 * Defines the following overloads:
 *     none_of(InputIterator, InputIterator, UnaryPredicate);
 *     none_of(Range,         UnaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    bool
    none_of(InputIterator  it_begin,
            InputIterator  it_end,
            UnaryPredicate pred)
    {
        return ::std::none_of(_utils::move(it_begin),
                              _utils::move(it_end),
                              _utils::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    bool
    none_of(Range &&       range,
            UnaryPredicate pred)
    {
        return ::std::none_of(begin(range),
                              end(range),
                              _utils::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_NONE_OF_HPP
