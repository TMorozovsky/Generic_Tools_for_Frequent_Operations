#ifndef GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP

#include <numeric>
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_subtraction.hpp"
#include "gtfo/_impl/type_traits/value_from_container.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/result_of_container_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator, typename OutputIterator>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::result_of_dereferencing< OutputIterator & >::type,
            typename _tt::result_of_subtraction
            <
                typename _tt::result_of_dereferencing< InputIterator & >::type,
                typename _tt::result_of_dereferencing< InputIterator & >::type
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(InputIterator  it_begin,
                        InputIterator  it_end,
                        OutputIterator it_out)
    {
        return ::std::adjacent_difference(it_begin, it_end, it_out);
    }

    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::result_of_dereferencing< OutputIterator & >::type,
            typename _tt::result_of_fun2
            <
                BinaryOperation,
                typename _tt::result_of_dereferencing< InputIterator & >::type,
                typename _tt::result_of_dereferencing< InputIterator & >::type
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(InputIterator   it_begin,
                        InputIterator   it_end,
                        OutputIterator  it_out,
                        BinaryOperation op)
    {
        return ::std::adjacent_difference(it_begin, it_end, it_out, op);
    }

    template<typename Container, typename OutputIterator>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::result_of_dereferencing< OutputIterator & >::type,
            typename _tt::result_of_subtraction
            <
                typename _tt::value_from_container< Container >::type &,
                typename _tt::value_from_container< Container >::type &
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(Container &&   container,
                        OutputIterator it_out)
    {
        return ::std::adjacent_difference(begin(container), end(container), it_out);
    }

    template<typename Container, typename OutputIterator, typename BinaryOperation>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            typename _tt::result_of_dereferencing< OutputIterator & >::type,
            typename _tt::result_of_fun2
            <
                BinaryOperation,
                typename _tt::value_from_container< Container >::type &,
                typename _tt::value_from_container< Container >::type &
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(Container &&    container,
                        OutputIterator  it_out,
                        BinaryOperation op)
    {
        return ::std::adjacent_difference(begin(container), end(container), it_out, op);
    }
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
