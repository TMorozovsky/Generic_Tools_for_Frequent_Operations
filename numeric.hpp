#ifndef GTFO_FILE_INCLUDED_NUMERIC_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_HPP

#include <numeric>
#include "type_traits.hpp"

namespace gtfo
{
    using ::std::accumulate;

    template<typename Container, typename Value>
    inline
    typename enable_if
    <
        can_add_elements_of_container_to_value<Container, Value>::value,
        Value
    >::type
    accumulate(const Container & container, Value init)
    {
        return ::std::accumulate(begin(container), end(container), init);
    }

    template<typename Container>
    inline
    typename enable_if
    <
        is_container<Container>::value,
        typename element_of_container<Container>::type
    >::type
    accumulate(const Container & container)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename element_of_container<Container>::type());
    }

    template<typename Container, typename Value, typename Function>
    inline
    typename enable_if
    <
        can_add_elements_of_container_to_value<Container, Value, Function>::value,
        Value
    >::type
    accumulate(const Container & container, Value init, Function fun)
    {
        return ::std::accumulate(begin(container), end(container), init, fun);
    }

    template<typename Container, typename Function>
    inline
    typename enable_if
    <
        invokable_on_2_elements_of_container<Function, Container>::value,
        typename element_of_container<Container>::type
    >::type
    accumulate(const Container & container, Function fun)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename element_of_container<Container>::type(), fun);
    }
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_HPP

