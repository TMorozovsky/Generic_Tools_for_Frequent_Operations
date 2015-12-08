#ifndef GTFO_FILE_INCLUDED_NUMERIC_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_HPP

#include <numeric>
#include "type_traits.hpp"

namespace gtfo
{
    using ::std::accumulate;

    template<typename Container, typename Value>
    inline
    typename tt::enable_if
    <
        tt::can_add_elements_of_container_to_value<Container, Value>::value,
        Value
    >::type
    accumulate(const Container & container, Value init)
    {
        return ::std::accumulate(begin(container), end(container), init);
    }

    template<typename Container>
    inline
    typename tt::enable_if
    <
        tt::is_container<Container>::value,
        typename tt::element_of_container<Container>::type
    >::type
    accumulate(const Container & container)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename tt::element_of_container<Container>::type());
    }

    template<typename Container, typename Value, typename BinaryOperation>
    inline
    typename tt::enable_if
    <
        tt::can_add_elements_of_container_to_value<Container, Value, BinaryOperation>::value,
        Value
    >::type
    accumulate(const Container & container, Value init, BinaryOperation op)
    {
        return ::std::accumulate(begin(container), end(container), init, op);
    }

    template<typename Container, typename BinaryOperation>
    inline
    typename tt::enable_if
    <
        tt::can_invoke_operation_on_two_container_elements<BinaryOperation, Container>::value,
        typename tt::element_of_container<Container>::type
    >::type
    accumulate(const Container & container, BinaryOperation op)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename tt::element_of_container<Container>::type(), op);
    }

    using ::std::adjacent_difference;

    template<typename Container, typename OutputIterator>
    inline
    typename tt::enable_if
    <
        tt::can_assign_container_element_to_dereferenced_output_iterator<Container, OutputIterator>::value,
        OutputIterator
    >::type
    adjacent_difference(const Container & container, OutputIterator output_iterator)
    {
        return ::std::adjacent_difference(begin(container), end(container), output_iterator);
    }

    template<typename Container, typename OutputIterator, typename BinaryOperation>
    inline
    typename tt::enable_if
    <
        tt::can_assign_container_element_to_dereferenced_output_iterator<Container, OutputIterator>::value
        && tt::can_invoke_operation_on_two_container_elements<BinaryOperation, Container>::value,
        OutputIterator
    >::type
    adjacent_difference(const Container & container, OutputIterator output_iterator, BinaryOperation op)
    {
        return ::std::adjacent_difference(begin(container), end(container), output_iterator, op);
    }
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_HPP
