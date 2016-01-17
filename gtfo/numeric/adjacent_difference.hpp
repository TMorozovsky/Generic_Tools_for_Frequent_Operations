#ifndef GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP

/*
 * Defines the following overloads:
 *     adjacent_difference(InputIterator, InputIterator,  OutputIterator);
 *     adjacent_difference(InputIterator, InputIterator,  OutputIterator, BinaryOperation);
 *     adjacent_difference(Range,         OutputIterator);
 *     adjacent_difference(Range,         OutputIterator, BinaryOperation);
 */

#include <numeric>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_subtraction.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

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
        return ::std::adjacent_difference(_utils::move(it_begin),
                                          _utils::move(it_end),
                                          _utils::move(it_out));
    }

    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    inline
    OutputIterator
    adjacent_difference(InputIterator   it_begin,
                        InputIterator   it_end,
                        OutputIterator  it_out,
                        BinaryOperation op)
    {
        return ::std::adjacent_difference(it_begin, it_end, it_out, op);
    }

    template<typename Range, typename OutputIterator>
    inline
    OutputIterator
    adjacent_difference(Range &&       range,
                        OutputIterator it_out)
    {
        return ::std::adjacent_difference(begin(range),
                                          end(range),
                                          _utils::move(it_out));
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
                                          _utils::move(it_out),
                                          _utils::move(op));
    }

#undef GTFO_RESULT_OF_ADJACENT_DIFFERENCE_OP
#undef GTFO_RESULT_OF_ADJACENT_DIFFERENCE

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ADJACENT_DIFFERENCE_HPP
