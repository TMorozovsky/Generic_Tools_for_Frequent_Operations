#ifndef GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP

#include <numeric>
#include "_impl/type_traits/is_assignable.hpp"
#include "_impl/type_traits/result_of_addition.hpp"
#include "_impl/type_traits/result_of_dereferencing.hpp"
#include "_impl/type_traits/iterator_of_container.hpp"
#include "_impl/type_traits/value_from_container.hpp"
#include "_impl/type_traits/result_of_fun2.hpp"

namespace gtfo
{
    template<typename Container, typename Value>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_addition
            <
                Value &,
                typename _tt::result_of_dereferencing
                <
                    typename _tt::iterator_of_container<Container>::type
                >::type
            >::type
        >::value,
        Value
    >::type
    accumulate(Container && container, Value init)
    {
        return ::std::accumulate(begin(container), end(container), init);
    }

    template<typename Container>
    inline
    typename _tt::value_from_container<Container>::type
    accumulate(Container && container)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename _tt::value_from_container<Container>::type());
    }

    template<typename Container, typename Value, typename BinaryOperation>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_fun2
            <
                BinaryOperation &,
                Value &,
                typename _tt::result_of_dereferencing
                <
                    typename _tt::iterator_of_container<Container>::type
                >::type
            >::type
        >::value,
        Value
    >::type
    accumulate(Container && container, Value init, BinaryOperation op)
    {
        return ::std::accumulate(begin(container), end(container), init, op);
    }

    template<typename Container, typename BinaryOperation>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::value_from_container<Container>::type &,
            typename _tt::result_of_fun2
            <
                BinaryOperation,
                typename _tt::value_from_container<Container>::type,
                typename _tt::value_from_container<Container>::type
            >::type
        >::value,
        typename _tt::value_from_container<Container>::type
    >::type
    accumulate(Container && container, BinaryOperation op)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename _tt::value_from_container<Container>::type(), op);
    }
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
