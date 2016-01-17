#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_HPP

/*
 * Defines the following overloads:
 *     find_if(InputIterator, InputIterator,  UnaryPredicate);
 *     find_if(Range,         UnaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    InputIterator
    find_if(InputIterator it_begin,
            InputIterator it_end,
            UnaryPredicate pred)
    {
        return ::std::find_if(_utils::move(it_begin),
                              _utils::move(it_end),
                              _utils::move(pred));
    }

    template<typename Range, typename UnaryPredicate>
    inline
    typename _tt::iterator_of_range< Range >::type
    find_if(Range &&      range,
            UnaryPredicate pred)
    {
        return ::std::find_if(begin(range),
                              end(range),
                              _utils::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_IF_HPP
