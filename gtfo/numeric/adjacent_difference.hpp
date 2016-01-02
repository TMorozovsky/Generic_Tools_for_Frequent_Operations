#ifndef GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP

#include <numeric>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_subtraction.hpp"
#include "gtfo/_impl/type_traits/value_from_range.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/result_of_range_iterator_dereferencing.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_ADJACENT_DIFFERENCE(InputIterator, OutputIterator) \
    typename _tt::enable_if                                                \
    <                                                                       \
        _tt::is_assignable                                                   \
        <                                                                     \
            typename _tt::result_of_dereferencing< OutputIterator >::type,     \
            typename _tt::result_of_subtraction                                 \
            <                                                                    \
                typename _tt::result_of_dereferencing< InputIterator >::type,    \
                typename _tt::result_of_dereferencing< InputIterator >::type     \
            >::type                                                              \
        >::value,                                                              \
        OutputIterator                                                       \
    >::type

#define GTFO_RESULT_OF_ADJACENT_DIFFERENCE_OP(InputIterator, OutputIterator, BinaryOperation) \
    typename _tt::enable_if                                               \
    <                                                                      \
        _tt::is_assignable                                                  \
        <                                                                    \
            typename _tt::result_of_dereferencing< OutputIterator >::type,    \
            typename _tt::result_of_fun2                                       \
            <                                                                   \
                BinaryOperation,                                                 \
                typename _tt::result_of_dereferencing< InputIterator >::type,    \
                typename _tt::result_of_dereferencing< InputIterator >::type     \
            >::type                                                              \
        >::value,                                                              \
        OutputIterator                                                       \
    >::type

    template<typename InputIterator, typename OutputIterator>
    inline
    GTFO_RESULT_OF_ADJACENT_DIFFERENCE(InputIterator,
                                       OutputIterator)
    adjacent_difference(InputIterator  it_begin,
                        InputIterator  it_end,
                        OutputIterator it_out)
    {
        return ::std::adjacent_difference(::gtfo::move(it_begin),
                                          ::gtfo::move(it_end),
                                          ::gtfo::move(it_out));
    }

    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_ADJACENT_DIFFERENCE_OP(InputIterator,
                                          OutputIterator,
                                          BinaryOperation)
    adjacent_difference(InputIterator   it_begin,
                        InputIterator   it_end,
                        OutputIterator  it_out,
                        BinaryOperation op)
    {
        return ::std::adjacent_difference(it_begin, it_end, it_out, op);
    }

    template<typename Range, typename OutputIterator>
    inline
    GTFO_RESULT_OF_ADJACENT_DIFFERENCE(typename _tt::iterator_of_range< Range >::type,
                                       OutputIterator)
    adjacent_difference(Range &&       range,
                        OutputIterator it_out)
    {
        return ::std::adjacent_difference(begin(range),
                                          end(range),
                                          ::gtfo::move(it_out));
    }

    template<typename Range, typename OutputIterator, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_ADJACENT_DIFFERENCE_OP(typename _tt::iterator_of_range< Range >::type,
                                          OutputIterator,
                                          BinaryOperation)
    adjacent_difference(Range &&        range,
                        OutputIterator  it_out,
                        BinaryOperation op)
    {
        return ::std::adjacent_difference(begin(range),
                                          end(range),
                                          ::gtfo::move(it_out),
                                          ::gtfo::move(op));
    }

#undef GTFO_RESULT_OF_ADJACENT_DIFFERENCE_OP
#undef GTFO_RESULT_OF_ADJACENT_DIFFERENCE

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
