#ifndef GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP

#include <numeric>
#include "type_traits.hpp"

namespace gtfo
{
#define GTFO_DISABLE_ADL(t) typename tt::type_of<t>::type

    template<typename InputIterator, typename OutputIterator>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            typename tt::result_of_dereferencing<OutputIterator>::type,
            typename tt::value_of_dereferenced<InputIterator>::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(GTFO_DISABLE_ADL(InputIterator)  it_begin,
                        GTFO_DISABLE_ADL(InputIterator)  it_end,
                        GTFO_DISABLE_ADL(OutputIterator) it_out)
    {
        return ::std::adjacent_difference(it_begin, it_end, it_out);
    }

    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            typename tt::result_of_dereferencing<OutputIterator>::type,
            typename tt::value_of_dereferenced<InputIterator>::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(GTFO_DISABLE_ADL(InputIterator)   it_begin,
                        GTFO_DISABLE_ADL(InputIterator)   it_end,
                        GTFO_DISABLE_ADL(OutputIterator)  it_out,
                        GTFO_DISABLE_ADL(BinaryOperation) op)
    {
        return ::std::adjacent_difference(it_begin, it_end, it_out, op);
    }

    template<typename Container, typename OutputIterator>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            typename tt::result_of_dereferencing<OutputIterator>::type,
            typename tt::result_of_subtraction
            <
                typename tt::value_from_container<Container>::type,
                typename tt::value_from_container<Container>::type
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
    typename tt::enable_if
    <
        tt::is_assignable
        <
            typename tt::result_of_dereferencing<OutputIterator>::type,
            typename tt::result_of_2
            <
                BinaryOperation,
                typename tt::value_from_container<Container>::type,
                typename tt::value_from_container<Container>::type
            >::type
        >::value,
        OutputIterator
    >::type
    adjacent_difference(Container && container, OutputIterator it, BinaryOperation op)
    {
        return ::std::adjacent_difference(begin(container), end(container), it, op);
    }

#undef GTFO_DISABLE_ADL
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
