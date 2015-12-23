#ifndef GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP

#include <numeric>
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_addition.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/value_from_container.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/result_of_container_iterator_dereferencing.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_ACCUMULATE(InputIterator, Value) \
    typename _tt::enable_if                                  \
    <                                                            \
        _tt::is_assignable                                           \
        <                                                               \
            Value,                                                         \
            typename _tt::result_of_addition                                 \
            <                                                                  \
                Value,                                                          \
                typename _tt::result_of_dereferencing< InputIterator >::type    \
            >::type                                                             \
        >::value,                                                              \
        Value                                                                \
    >::type

#define GTFO_RESULT_OF_ACCUMULATE_OP(InputIterator, Value, BinaryOperation) \
    typename _tt::enable_if                             \
    <                                                        \
        _tt::is_assignable                                       \
        <                                                            \
            Value,                                                      \
            typename _tt::result_of_fun2                                   \
            <                                                                \
                BinaryOperation,                                               \
                Value,                                                          \
                typename _tt::result_of_dereferencing< InputIterator >::type    \
            >::type                                                             \
        >::value,                                                              \
        Value                                                                \
    >::type

    template<typename InputIterator, typename Value>
    inline
    GTFO_RESULT_OF_ACCUMULATE(InputIterator,
                              Value)
    accumulate(InputIterator it_begin,
               InputIterator it_end,
               Value         init)
    {
        return ::std::accumulate(it_begin, it_end, init);
    }

    template<typename InputIterator, typename Value, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_ACCUMULATE_OP(InputIterator,
                                 Value,
                                 BinaryOperation)
    accumulate(InputIterator   it_begin,
               InputIterator   it_end,
               Value           init,
               BinaryOperation op)
    {
        return ::std::accumulate(it_begin, it_end, init, op);
    }


    template<typename Container>
    inline
    GTFO_RESULT_OF_ACCUMULATE(typename _tt::iterator_of_container< Container >::type,
                              typename _tt::value_from_container< Container >::type)
    accumulate(Container && container)
    {
        return ::std::accumulate(begin(container), end(container),
                                 typename _tt::value_from_container<Container>::type());
    }

    template<typename Container, typename Value>
    inline
    GTFO_RESULT_OF_ACCUMULATE(typename _tt::iterator_of_container< Container >::type,
                              Value)
    accumulate(Container && container,
               Value        init)
    {
        return ::std::accumulate(begin(container), end(container), init);
    }

    template<typename Container, typename Value, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_ACCUMULATE_OP(typename _tt::iterator_of_container< Container >::type,
                                 Value,
                                 BinaryOperation)
    accumulate(Container &&    container,
               Value           init,
               BinaryOperation op)
    {
        return ::std::accumulate(begin(container), end(container), init, op);
    }

#undef GTFO_RESULT_OF_ACCUMULATE_OP
#undef GTFO_RESULT_OF_ACCUMULATE

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
