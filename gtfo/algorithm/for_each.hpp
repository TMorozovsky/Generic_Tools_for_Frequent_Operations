#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_invokable_fun1.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_range_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryFunction>
    inline
    typename _tt::enable_if
    <
        _tt::is_invokable_fun1
        <
            UnaryFunction,
            typename _tt::result_of_dereferencing< InputIterator >::type
        >::value,
        UnaryFunction
    >::type
    for_each(InputIterator it_begin,
             InputIterator it_end,
             UnaryFunction fun)
    {
        return ::std::for_each(::gtfo::move(it_begin),
                               ::gtfo::move(it_end),
                               ::gtfo::move(fun));
    }

    template<typename Range, typename UnaryFunction>
    inline
    typename _tt::enable_if
    <
        _tt::is_invokable_fun1
        <
            UnaryFunction,
            typename _tt::result_of_range_iterator_dereferencing< Range >::type
        >::value,
        UnaryFunction
    >::type
    for_each(Range &&      range,
             UnaryFunction fun)
    {
        return ::std::for_each(begin(range),
                               end(range),
                               ::gtfo::move(fun));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
