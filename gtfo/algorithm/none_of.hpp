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
    none_of(InputIterator  _it_begin,
            InputIterator  _it_end,
            UnaryPredicate _pred)
    {
        return ::std::none_of(::gtfo::move(_it_begin),
                              ::gtfo::move(_it_end),
                              ::gtfo::move(_pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    bool
    none_of(Range &&       _range,
            UnaryPredicate _pred)
    {
        return ::std::none_of(begin(_range),
                              end(_range),
                              ::gtfo::move(_pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_NONE_OF_HPP
