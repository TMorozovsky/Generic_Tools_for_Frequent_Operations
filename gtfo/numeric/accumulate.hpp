#ifndef GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP

/*
 * Defines the following overloads:
 *     accumulate(InputIterator, InputIterator);
 *     accumulate(InputIterator, InputIterator, Value);
 *     accumulate(InputIterator, InputIterator, Value,           BinaryOperation);
 *     accumulate(Range);
 *     accumulate(Range,         Value);
 *     accumulate(Range,         Value,         BinaryOperation);
 */

#include <numeric>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_addition.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"
#include "gtfo/_impl/type_traits/value_from_range.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"

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

    template<typename InputIterator>
    inline
    GTFO_RESULT_OF_ACCUMULATE(InputIterator,
                              typename _tt::value_of_dereferenced< InputIterator >::type)
    accumulate(InputIterator it_begin,
               InputIterator it_end)
    {
        return ::std::accumulate(_utils::move(it_begin),
                                 _utils::move(it_end),
                                 typename _tt::value_of_dereferenced< InputIterator >::type());
    }

    template<typename InputIterator, typename Value>
    inline
    GTFO_RESULT_OF_ACCUMULATE(InputIterator,
                              Value)
    accumulate(InputIterator it_begin,
               InputIterator it_end,
               Value         init)
    {
        return ::std::accumulate(_utils::move(it_begin),
                                 _utils::move(it_end),
                                 _utils::move(init));
    }

    template<typename InputIterator, typename Value, typename BinaryOperation>
    inline
    Value
    accumulate(InputIterator   it_begin,
               InputIterator   it_end,
               Value           init,
               BinaryOperation op)
    {
        return ::std::accumulate(_utils::move(it_begin),
                                 _utils::move(it_end),
                                 _utils::move(init),
                                 _utils::move(op));
    }

    template<typename Range>
    inline
    typename _tt::value_from_range< Range >::type
    accumulate(Range && range)
    {
        return ::std::accumulate(begin(range),
                                 end(range),
                                 typename _tt::value_from_range< Range >::type());
    }

    template<typename Range, typename Value>
    inline
    GTFO_RESULT_OF_ACCUMULATE(typename _tt::iterator_of_range< Range >::type,
                              Value)
    accumulate(Range && range,
               Value    init)
    {
        return ::std::accumulate(begin(range),
                                 end(range),
                                 _utils::move(init));
    }

    template<typename Range, typename Value, typename BinaryOperation>
    inline
    GTFO_RESULT_OF_ACCUMULATE_OP(typename _tt::iterator_of_range< Range >::type,
                                 Value,
                                 BinaryOperation)
    accumulate(Range &&        range,
               Value           init,
               BinaryOperation op)
    {
        return ::std::accumulate(begin(range),
                                 end(range),
                                 _utils::move(init),
                                 _utils::move(op));
    }

#undef GTFO_RESULT_OF_ACCUMULATE_OP
#undef GTFO_RESULT_OF_ACCUMULATE

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_ACCUMULATE_HPP
