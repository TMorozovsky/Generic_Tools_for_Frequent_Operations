#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_HPP

/*
 * Defines the following overloads:
 *     find(InputIterator, InputIterator, Value);
 *     find(Range,         Value);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{
    template<typename InputIterator, typename Value>
    inline
    InputIterator
    find(InputIterator it_begin,
         InputIterator it_end,
         const Value & value)
    {
        return ::std::find(_utils::move(it_begin),
                           _utils::move(it_end),
                           value);
    }

    template<typename Range, typename Value>
    inline
    typename _tt::iterator_of_range< Range >::type
    find(Range &&      range,
         const Value & value)
    {
        return ::std::find(begin(range),
                           end(range),
                           value);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_HPP
