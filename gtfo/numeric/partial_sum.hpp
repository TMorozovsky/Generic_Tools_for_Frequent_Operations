#ifndef GTFO_FILE_INCLUDED_NUMERIC_PARTIAL_SUM_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_PARTIAL_SUM_HPP

#include <numeric>
#include "gtfo/_impl/move.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"
#include "gtfo/_impl/type_traits/result_of_addition.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_PARTIAL_SUM(InputIterator, OutputIterator) \
    typename _tt::enable_if                                          \
    <                                                                   \
        _tt::is_assignable                                                 \
        <                                                                     \
            typename _tt::value_of_dereferenced< InputIterator >::type,         \
            typename _tt::result_of_addition                                     \
            <                                                                     \
                typename _tt::value_of_dereferenced< InputIterator >::type,       \
                typename _tt::result_of_dereferencing< InputIterator >::type      \
            >::type                                                               \
        >::value                                                                  \
        &&                                                                        \
        _tt::is_assignable                                                       \
        <                                                                       \
            typename _tt::result_of_dereferencing< OutputIterator >::type,     \
            typename _tt::value_of_dereferenced< InputIterator >::type        \
        >::value,                                                           \
        OutputIterator                                                   \
    >::type

#define GTFO_RESULT_OF_PARTIAL_SUM_OP(InputIterator, OutputIterator, BinaryOperation) \
    typename _tt::enable_if                                          \
    <                                                                   \
        _tt::is_assignable                                                 \
        <                                                                     \
            typename _tt::value_of_dereferenced< InputIterator >::type,         \
            typename _tt::result_of_fun2                                         \
            <                                                                     \
                BinaryOperation,                                                  \
                typename _tt::value_of_dereferenced< InputIterator >::type,       \
                typename _tt::result_of_dereferencing< InputIterator >::type      \
            >::type                                                               \
        >::value                                                                  \
        &&                                                                        \
        _tt::is_assignable                                                       \
        <                                                                       \
            typename _tt::result_of_dereferencing< OutputIterator >::type,     \
            typename _tt::value_of_dereferenced< InputIterator >::type        \
        >::value,                                                           \
        OutputIterator                                                   \
    >::type

    template<typename InputIterator, typename OutputIterator>
    inline
    GTFO_RESULT_OF_PARTIAL_SUM(InputIterator,
                               OutputIterator)
    partial_sum(InputIterator  it_begin,
                InputIterator  it_end,
                OutputIterator it_dest)
    {
        return ::std::partial_sum(::gtfo::move(it_begin),
                                  ::gtfo::move(it_end),
                                  ::gtfo::move(it_dest));
    }

    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_PARTIAL_SUM_OP(InputIterator,
                                  OutputIterator,
                                  BinaryOperation)
    partial_sum(InputIterator   it_begin,
                InputIterator   it_end,
                OutputIterator  it_dest,
                BinaryOperation op)
    {
        return ::std::partial_sum(::gtfo::move(it_begin),
                                  ::gtfo::move(it_end),
                                  ::gtfo::move(it_dest),
                                  ::gtfo::move(op));
    }

    template<typename Container, typename OutputIterator>
    inline
    GTFO_RESULT_OF_PARTIAL_SUM(typename _tt::iterator_of_container< Container >::type,
                               OutputIterator)
    partial_sum(Container &&   container,
                OutputIterator it_dest)
    {
        return ::std::partial_sum(begin(container),
                                  end(container),
                                  ::gtfo::move(it_dest));
    }

    template<typename Container, typename OutputIterator, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_PARTIAL_SUM_OP(typename _tt::iterator_of_container< Container >::type,
                                  OutputIterator,
                                  BinaryOperation)
    partial_sum(Container &&    container,
                OutputIterator  it_dest,
                BinaryOperation op)
    {
        return ::std::partial_sum(begin(container),
                                  end(container),
                                  ::gtfo::move(it_dest),
                                  ::gtfo::move(op));
    }

#undef GTFO_RESULT_OF_PARTIAL_SUM_OP
#undef GTFO_RESULT_OF_PARTIAL_SUM

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_PARTIAL_SUM_HPP

