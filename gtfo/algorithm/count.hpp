#ifndef GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP

/*
 * Defines the following overloads:
 *     count(InputIterator, InputIterator, Value);
 *     count(Range,         Value);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/result_of_begin.hpp"
#include "gtfo/_impl/type_traits/result_of_end.hpp"

namespace gtfo
{
    template<typename InputIterator, typename Value>
    inline
    auto
    count(InputIterator it_begin,
          InputIterator it_end,
          const Value & value) -> decltype(::std::count(_tt::declval< InputIterator >(),
                                                        _tt::declval< InputIterator >(),
                                                        _tt::declval< const Value & >()))
    {
        return ::std::count(_utils::move(it_begin),
                            _utils::move(it_end),
                            value);
    }

    template<typename Range, typename Value>
    inline
    auto
    count(Range &&      range,
          const Value & value) -> decltype(::std::count(_tt::declval< typename _tt::result_of_begin< Range >::type >(),
                                                        _tt::declval< typename _tt::result_of_end< Range >::type >(),
                                                        _tt::declval< const Value & >()))
    {
        return ::std::count(begin(range),
                            end(range),
                            value);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP
