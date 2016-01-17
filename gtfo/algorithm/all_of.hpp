#ifndef GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP

/*
 * Defines the following overloads:
 *     all_of(InputIterator, InputIterator, UnaryPredicate);
 *     all_of(Range,         UnaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    bool
    all_of(InputIterator  it_begin,
           InputIterator  it_end,
           UnaryPredicate pred)
    {
        return ::std::all_of(_utils::move(it_begin),
                             _utils::move(it_end),
                             _utils::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    bool
    all_of(Range &&       range,
           UnaryPredicate pred)
    {
        return ::std::all_of(begin(range),
                             end(range),
                             _utils::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP
