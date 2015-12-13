#ifndef GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP

#include <numeric>
#include "_impl/type_traits/is_assignable.hpp"
#include "_impl/type_traits/result_of_dereferencing.hpp"
#include "_impl/type_traits/result_of_subtraction.hpp"
#include "_impl/type_traits/value_from_container.hpp"
#include "_impl/type_traits/result_of_fun2.hpp"

namespace gtfo
{
    template<typename Container, typename OutputIterator>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::result_of_dereferencing<OutputIterator>::type,
            typename _tt::result_of_subtraction
            <
                typename _tt::value_from_container<Container>::type,
                typename _tt::value_from_container<Container>::type
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(Container && container, OutputIterator it)
    {
        return ::std::adjacent_difference(begin(container), end(container), it);
    }

    template<typename Container, typename OutputIterator, typename BinaryOperation>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::result_of_dereferencing<OutputIterator>::type,
            typename _tt::result_of_fun2
            <
                BinaryOperation,
                typename _tt::value_from_container<Container>::type,
                typename _tt::value_from_container<Container>::type
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(Container && container, OutputIterator it, BinaryOperation op)
    {
        return ::std::adjacent_difference(begin(container), end(container), it, op);
    }
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
