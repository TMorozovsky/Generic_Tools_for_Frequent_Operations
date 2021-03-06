#ifndef GTFO_FILE_INCLUDED_ALGORITHM_ANY_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_ANY_OF_HPP

/*
 * Defines the following overloads:
 *     any_of(InputIterator, InputIterator, UnaryPredicate);
 *     any_of(Range,         UnaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    bool
    any_of(InputIterator  _it_begin,
           InputIterator  _it_end,
           UnaryPredicate _pred)
    {
        return ::std::any_of(::gtfo::move(_it_begin),
                             ::gtfo::move(_it_end),
                             ::gtfo::move(_pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    bool
    any_of(Range &&       _range,
           UnaryPredicate _pred)
    {
        return ::std::any_of(begin(_range),
                             end(_range),
                             ::gtfo::move(_pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_ANY_OF_HPP
