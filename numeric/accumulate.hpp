#ifndef GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP

#include <numeric>
#include "type_traits.hpp"

namespace gtfo
{
#define GTFO_DISABLE_ADL(t) typename tt::type_of<t>::type

    template<typename InputIterator, typename Value>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            Value &,
            typename tt::result_of_addition
            <
                Value &,
                typename tt::result_of_dereferencing<InputIterator>::type
            >::type
        >::value,
        Value
    >::type
    accumulate(GTFO_DISABLE_ADL(InputIterator) it_begin,
               GTFO_DISABLE_ADL(InputIterator) it_end,
               GTFO_DISABLE_ADL(Value)         init)
    {
        return ::std::accumulate(it_begin, it_end, init);
    }

    template<typename InputIterator, typename Value, typename BinaryOperation>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            Value &,
            typename tt::result_of_2
            <
                BinaryOperation &,
                Value &,
                typename tt::result_of_dereferencing<InputIterator>::type
            >::type
        >::value,
        Value
    >::type
    accumulate(GTFO_DISABLE_ADL(InputIterator)   it_begin,
               GTFO_DISABLE_ADL(InputIterator)   it_end,
               GTFO_DISABLE_ADL(Value)           init,
               GTFO_DISABLE_ADL(BinaryOperation) op)
    {
        return ::std::accumulate(it_begin, it_end, init, op);
    }

    template<typename Container, typename Value>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            Value &,
            typename tt::result_of_addition
            <
                Value &,
                typename tt::result_of_dereferencing
                <
                    typename tt::iterator_of_container<Container>::type
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
    typename tt::value_from_container<Container>::type
    accumulate(Container && container)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename tt::value_from_container<Container>::type());
    }

    template<typename Container, typename Value, typename BinaryOperation>
    inline
    typename tt::enable_if
    <
        tt::is_assignable
        <
            Value &,
            typename tt::result_of_2
            <
                BinaryOperation &,
                Value &,
                typename tt::result_of_dereferencing
                <
                    typename tt::iterator_of_container<Container>::type
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
    typename tt::enable_if
    <
        tt::is_invokable_2
        <
            BinaryOperation,
            typename tt::value_from_container<Container>::type,
            typename tt::value_from_container<Container>::type
        >::value,
        typename tt::value_from_container<Container>::type
    >::type
    accumulate(const Container & container, BinaryOperation op)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename tt::value_from_container<Container>::type(), op);
    }

#undef GTFO_DISABLE_ADL
}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
