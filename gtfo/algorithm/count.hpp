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
    count(InputIterator _it_begin,
          InputIterator _it_end,
          const Value & _value) -> decltype(::std::count(_tt::declval< InputIterator >(),
                                                         _tt::declval< InputIterator >(),
                                                         _tt::declval< const Value & >()))
    {
        return ::std::count(_utils::move(_it_begin),
                            _utils::move(_it_end),
                            _value);
    }

    template<typename Range, typename Value>
    inline
    auto
    count(Range &&      _range,
          const Value & _value) -> decltype(::std::count(_tt::declval< typename _tt::result_of_begin< Range >::type >(),
                                                         _tt::declval< typename _tt::result_of_end< Range >::type >(),
                                                         _tt::declval< const Value & >()))
    {
        return ::std::count(begin(_range),
                            end(_range),
                            _value);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_COUNT_HPP
