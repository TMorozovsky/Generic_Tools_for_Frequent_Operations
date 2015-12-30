#ifndef GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP

#include <algorithm>
#include "gtfo/_impl/move.hpp"
#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"
#include "gtfo/_impl/type_traits/result_of_fun1.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_container_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator, typename UnaryPredicate>
    inline
    typename _tt::enable_if
    <
        _tt::can_be_used_in_boolean_context
        <
            typename _tt::result_of_fun1
            <
                UnaryPredicate,
                typename _tt::result_of_dereferencing< InputIterator >::type
            >::type
        >::value,
        bool
    >::type
    all_of(InputIterator  it_begin,
           InputIterator  it_end,
           UnaryPredicate pred)
    {
        return ::std::all_of(::gtfo::move(it_begin),
                             ::gtfo::move(it_end),
                             ::gtfo::move(pred));
    }

    template<typename Container, typename UnaryPredicate>
    inline
    typename _tt::enable_if
    <
        _tt::can_be_used_in_boolean_context
        <
            typename _tt::result_of_fun1
            <
                UnaryPredicate,
                typename _tt::result_of_container_iterator_dereferencing< Container >::type
            >::type
        >::value,
        bool
    >::type
    all_of(Container &&   container,
           UnaryPredicate pred)
    {
        return ::std::all_of(begin(container),
                             end(container),
                             ::gtfo::move(pred));
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_ALL_OF_HPP
