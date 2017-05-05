#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_NOT_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_NOT_HPP

/*
 * Defines the following overloads:
 *     find_if_not(InputIterator, InputIterator,  UnaryPredicate);
 *     find_if_not(Range,         UnaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    InputIterator
    find_if_not(InputIterator it_begin,
                InputIterator it_end,
                UnaryPredicate pred)
    {
        return ::std::find_if_not(::gtfo::move(it_begin),
                                  ::gtfo::move(it_end),
                                  ::gtfo::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    typename _tt::iterator_of_range< Range >::type
    find_if_not(Range &&      range,
                UnaryPredicate pred)
    {
        return ::std::find_if_not(begin(range),
                                  end(range),
                                  ::gtfo::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_NOT_HPP
