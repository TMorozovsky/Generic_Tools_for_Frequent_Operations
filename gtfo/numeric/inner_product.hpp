#ifndef GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP

#include <numeric>
#include "gtfo/_impl/move.hpp"
#include "gtfo/_impl/type_traits/common_type_2.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_addition.hpp"
#include "gtfo/_impl/type_traits/result_of_multiplication.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/is_range_castable_to_its_iterator.hpp"
#include "gtfo/_impl/type_traits/result_of_range_iterator_dereferencing.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_INNER_PRODUCT(InputIterator1, InputIterator2, Value) \
    typename _tt::enable_if                                                  \
    <                                                                         \
        _tt::is_assignable                                                     \
        <                                                                       \
            Value,                                                               \
            typename _tt::result_of_addition                                      \
            <                                                                      \
                Value,                                                              \
                typename _tt::result_of_multiplication                               \
                <                                                                    \
                    typename _tt::result_of_dereferencing< InputIterator1 >::type,   \
                    typename _tt::result_of_dereferencing< InputIterator2 >::type    \
                >::type                                                              \
            >::type                                                                 \
        >::value,                                                                  \
        Value                                                                     \
    >::type

#define GTFO_RESULT_OF_INNER_PRODUCT_OPS(InputIterator1, InputIterator2, Value, BinaryOperation1, BinaryOperation2) \
    typename _tt::enable_if                                               \
    <                                                                      \
        _tt::is_assignable                                                  \
        <                                                                    \
            Value,                                                            \
            typename _tt::result_of_fun2                                       \
            <                                                                   \
                BinaryOperation1,                                                \
                Value,                                                            \
                typename _tt::result_of_fun2                                       \
                <                                                                   \
                    BinaryOperation2,                                                \
                    typename _tt::result_of_dereferencing< InputIterator1 >::type,   \
                    typename _tt::result_of_dereferencing< InputIterator2 >::type    \
                >::type                                                              \
            >::type                                                                 \
        >::value,                                                                  \
        Value                                                                     \
    >::type

#define GTFO_ITER_COMMON_VALUE(InputIterator1, InputIterator2) \
    typename _tt::common_type_2                                    \
    <                                                                 \
        typename _tt::result_of_dereferencing< InputIterator1 >::type,  \
        typename _tt::result_of_dereferencing< InputIterator2 >::type   \
    >::type

    template<typename InputIterator1, typename InputIterator2, typename Value>
    inline
    GTFO_RESULT_OF_INNER_PRODUCT(InputIterator1,
                                 InputIterator2,
                                 Value)
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  InputIterator2 it_begin_2,
                  Value          init)
    {
        return ::std::inner_product(::gtfo::move(it_begin_1),
                                    ::gtfo::move(it_end_1),
                                    ::gtfo::move(it_begin_2),
                                    ::gtfo::move(init));
    }

    template<typename InputIterator1, typename InputIterator2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    GTFO_RESULT_OF_INNER_PRODUCT_OPS(InputIterator1,
                                     InputIterator2,
                                     Value,
                                     BinaryOperation1,
                                     BinaryOperation2)
    inner_product(InputIterator1   it_begin_1,
                  InputIterator1   it_end_1,
                  InputIterator2   it_begin_2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(::gtfo::move(it_begin_1),
                                    ::gtfo::move(it_end_1),
                                    ::gtfo::move(it_begin_2),
                                    ::gtfo::move(init),
                                    ::gtfo::move(op1),
                                    ::gtfo::move(op2));
    }

    template<typename InputIterator1, typename InputIterator2>
    inline
    GTFO_RESULT_OF_INNER_PRODUCT(InputIterator1,
                                 InputIterator2,
                                 GTFO_ITER_COMMON_VALUE(InputIterator1,
                                                        InputIterator2))
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  InputIterator2 it_begin_2)
    {
        return ::std::inner_product(::gtfo::move(it_begin_1),
                                    ::gtfo::move(it_end_1),
                                    ::gtfo::move(it_begin_2),
                                    GTFO_ITER_COMMON_VALUE(InputIterator1,
                                                           InputIterator2)());
    }

    template<typename Range1, typename InputIterator2, typename Value>
    inline
    GTFO_RESULT_OF_INNER_PRODUCT(typename _tt::iterator_of_range< Range1 >::type,
                                 InputIterator2,
                                 Value)
    inner_product(Range1 &&      range1,
                  InputIterator2 it_begin_2,
                  Value          init)
    {
        return ::std::inner_product(begin(range1),
                                    end(range1),
                                    ::gtfo::move(it_begin_2),
                                    ::gtfo::move(init));
    }

    template<typename Range1, typename InputIterator2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    GTFO_RESULT_OF_INNER_PRODUCT_OPS(typename _tt::iterator_of_range< Range1 >::type,
                                     InputIterator2,
                                     Value,
                                     BinaryOperation1,
                                     BinaryOperation2)
    inner_product(Range1 &&        range1,
                  InputIterator2   it_begin_2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(begin(range1),
                                    end(range1),
                                    ::gtfo::move(it_begin_2),
                                    ::gtfo::move(init),
                                    ::gtfo::move(op1),
                                    ::gtfo::move(op2));
    }

    template<typename InputIterator1, typename Range2, typename Value>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< Range2 >::value,
        GTFO_RESULT_OF_INNER_PRODUCT(InputIterator1,
                                     typename _tt::iterator_of_range< Range2 >::type,
                                     Value)
    >::type
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  Range2 &&      range2,
                  Value          init)
    {
        return ::std::inner_product(::gtfo::move(it_begin_1),
                                    ::gtfo::move(it_end_1),
                                    begin(range2),
                                    ::gtfo::move(init));
    }

    template<typename InputIterator1, typename Range2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< Range2 >::value,
        GTFO_RESULT_OF_INNER_PRODUCT_OPS(InputIterator1,
                                         typename _tt::iterator_of_range< Range2 >::type,
                                         Value,
                                         BinaryOperation1,
                                         BinaryOperation2)
    >::type
    inner_product(InputIterator1   it_begin_1,
                  InputIterator1   it_end_1,
                  Range2 &&        range2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(::gtfo::move(it_begin_1),
                                    ::gtfo::move(it_end_1),
                                    begin(range2),
                                    ::gtfo::move(init),
                                    ::gtfo::move(op1),
                                    ::gtfo::move(op2));
    }

    template<typename Range1, typename Range2, typename Value>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< Range2 >::value,
        GTFO_RESULT_OF_INNER_PRODUCT(typename _tt::iterator_of_range< Range1 >::type,
                                     typename _tt::iterator_of_range< Range2 >::type,
                                     Value)
    >::type
    inner_product(Range1 && range1,
                  Range2 && range2,
                  Value     init)
    {
        return ::std::inner_product(begin(range1),
                                    end(range1),
                                    begin(range2),
                                    ::gtfo::move(init));
    }

    template<typename Range1, typename Range2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< Range2 >::value,
        GTFO_RESULT_OF_INNER_PRODUCT_OPS(typename _tt::iterator_of_range< Range1 >::type,
                                         typename _tt::iterator_of_range< Range2 >::type,
                                         Value,
                                         BinaryOperation1,
                                         BinaryOperation2)
    >::type
    inner_product(Range1 &&        range1,
                  Range2 &&        range2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(begin(range1),
                                    end(range1),
                                    begin(range2),
                                    ::gtfo::move(init),
                                    ::gtfo::move(op1),
                                    ::gtfo::move(op2));
    }

    template<typename Range1, typename InputIterator2>
    inline
    GTFO_RESULT_OF_INNER_PRODUCT(typename _tt::iterator_of_range< Range1 >::type,
                                 InputIterator2,
                                 GTFO_ITER_COMMON_VALUE(typename _tt::iterator_of_range< Range1 >::type,
                                                        InputIterator2))
    inner_product(Range1 &&      range1,
                  InputIterator2 it_begin_2)
    {
        return ::std::inner_product(begin(range1),
                                    end(range1),
                                    ::gtfo::move(it_begin_2),
                                    GTFO_ITER_COMMON_VALUE(typename _tt::iterator_of_range< Range1 >::type,
                                                           InputIterator2)());
    }

    template<typename InputIterator1, typename Range2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< Range2 >::value,
        GTFO_RESULT_OF_INNER_PRODUCT(InputIterator1,
                                     typename _tt::iterator_of_range< Range2 >::type,
                                     GTFO_ITER_COMMON_VALUE(InputIterator1,
                                                            typename _tt::iterator_of_range< Range2 >::type))
    >::type
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  Range2 &&      range2)
    {
        return ::std::inner_product(::gtfo::move(it_begin_1),
                                    ::gtfo::move(it_end_1),
                                    begin(range2),
                                    GTFO_ITER_COMMON_VALUE(InputIterator1,
                                                           typename _tt::iterator_of_range< Range2 >::type)());
    }

    template<typename Range1, typename Range2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< Range2 >::value,
        GTFO_RESULT_OF_INNER_PRODUCT(typename _tt::iterator_of_range< Range1 >::type,
                                     typename _tt::iterator_of_range< Range2 >::type,
                                     GTFO_ITER_COMMON_VALUE(typename _tt::iterator_of_range< Range1 >::type,
                                                            typename _tt::iterator_of_range< Range2 >::type))
    >::type
    inner_product(Range1 && range1,
                  Range2 && range2)
    {
        return ::std::inner_product(begin(range1),
                                    end(range1),
                                    begin(range2),
                                    GTFO_ITER_COMMON_VALUE(typename _tt::iterator_of_range< Range1 >::type,
                                                           typename _tt::iterator_of_range< Range2 >::type)());
    }

#undef GTFO_ITER_COMMON_VALUE

#undef GTFO_RESULT_OF_INNER_PRODUCT_OPS
#undef GTFO_RESULT_OF_INNER_PRODUCT

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP
