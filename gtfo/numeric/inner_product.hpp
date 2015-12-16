#ifndef GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP

#include <numeric>
#include "gtfo/_impl/type_traits/common_type_2.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_addition.hpp"
#include "gtfo/_impl/type_traits/result_of_multiplication.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/iterator_of_container.hpp"
#include "gtfo/_impl/type_traits/is_container_castable_to_its_iterator.hpp"
#include "gtfo/_impl/type_traits/result_of_container_iterator_dereferencing.hpp"

namespace gtfo
{
    template<typename InputIterator1, typename InputIterator2, typename Value>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_addition
            <
                Value &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_dereferencing< InputIterator1 & >::type,
                    typename _tt::result_of_dereferencing< InputIterator2 & >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  InputIterator2 it_begin_2,
                  Value          init)
    {
        return ::std::inner_product(it_begin_1, it_end_1, it_begin_2,
                                    init);
    }

    template<typename InputIterator1, typename InputIterator2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_fun2
            <
                BinaryOperation1,
                Value &,
                typename _tt::result_of_fun2
                <
                    BinaryOperation2,
                    typename _tt::result_of_dereferencing< InputIterator1 & >::type,
                    typename _tt::result_of_dereferencing< InputIterator2 & >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(InputIterator1   it_begin_1,
                  InputIterator1   it_end_1,
                  InputIterator2   it_begin_2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(it_begin_1, it_end_1, it_begin_2,
                                    init, op1, op2);
    }

#define GTFO_COMMON_TYPE_VALUE typename _tt::common_type_2 \
                               < \
                                   typename _tt::result_of_dereferencing< InputIterator1 & >::type, \
                                   typename _tt::result_of_dereferencing< InputIterator2 & >::type \
                               >::type

    template<typename InputIterator1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            GTFO_COMMON_TYPE_VALUE &,
            typename _tt::result_of_addition
            <
                GTFO_COMMON_TYPE_VALUE &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_dereferencing< InputIterator1 & >::type,
                    typename _tt::result_of_dereferencing< InputIterator2 & >::type
                >::type
            >::type
        >::value,
        GTFO_COMMON_TYPE_VALUE
    >::type
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  InputIterator2 it_begin_2)
    {
        return ::std::inner_product(it_begin_1, it_end_1, it_begin_2,
                                    GTFO_COMMON_TYPE_VALUE());
    }

#undef GTFO_COMMON_TYPE_VALUE

    template<typename Container1, typename InputIterator2, typename Value>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_addition
            <
                Value &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_container_iterator_dereferencing< Container1 >::type,
                    typename _tt::result_of_dereferencing< InputIterator2 & >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(Container1 &&  container1,
                  InputIterator2 it_begin_2,
                  Value          init)
    {
        return ::std::inner_product(begin(container1), end(container1), it_begin_2, init);
    }

    template<typename Container1, typename InputIterator2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_fun2
            <
                BinaryOperation1,
                Value &,
                typename _tt::result_of_fun2
                <
                    BinaryOperation2,
                    typename _tt::result_of_container_iterator_dereferencing< Container1 >::type,
                    typename _tt::result_of_dereferencing< InputIterator2 & >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(Container1 &&    container1,
                  InputIterator2   it_begin_2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(begin(container1), end(container1), it_begin_2, init,
                                    op1, op2);
    }

    template<typename InputIterator1, typename Container2, typename Value>
    inline
    typename _tt::enable_if
    <
        !_tt::is_container_castable_to_its_iterator<Container2>::value
        &&
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_addition
            <
                Value &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_dereferencing< InputIterator1 & >::type,
                    typename _tt::result_of_container_iterator_dereferencing< Container2 >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  Container2 &&  container2,
                  Value          init)
    {
        return ::std::inner_product(it_begin_1, it_end_1, begin(container2), init);
    }

    template<typename InputIterator1, typename Container2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_container_castable_to_its_iterator<Container2>::value
        &&
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_fun2
            <
                BinaryOperation1,
                Value &,
                typename _tt::result_of_fun2
                <
                    BinaryOperation2,
                    typename _tt::result_of_dereferencing< InputIterator1 & >::type,
                    typename _tt::result_of_container_iterator_dereferencing< Container2 >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(InputIterator1   it_begin_1,
                  InputIterator1   it_end_1,
                  Container2 &&    container2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(it_begin_1, it_end_1, begin(container2), init, op1, op2);
    }

    template<typename Container1, typename Container2, typename Value>
    inline
    typename _tt::enable_if
    <
        !_tt::is_container_castable_to_its_iterator<Container2>::value
        &&
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_addition
            <
                Value &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_container_iterator_dereferencing< Container1 >::type,
                    typename _tt::result_of_container_iterator_dereferencing< Container2 >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(Container1 && container1,
                  Container2 && container2,
                  Value         init)
    {
        return ::std::inner_product(begin(container1), end(container1), begin(container2), init);
    }

    template<typename Container1, typename Container2, typename Value,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_container_castable_to_its_iterator<Container2>::value
        &&
        _tt::is_assignable
        <
            Value &,
            typename _tt::result_of_fun2
            <
                BinaryOperation1,
                Value &,
                typename _tt::result_of_fun2
                <
                    BinaryOperation2,
                    typename _tt::result_of_container_iterator_dereferencing< Container1 >::type,
                    typename _tt::result_of_container_iterator_dereferencing< Container2 >::type
                >::type
            >::type
        >::value,
        Value
    >::type
    inner_product(Container1 &&    container1,
                  Container2 &&    container2,
                  Value            init,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(begin(container1), end(container1), begin(container2), init, op1, op2);
    }

#define GTFO_COMMON_TYPE_VALUE \
    typename _tt::common_type_2 \
    < \
        typename _tt::result_of_container_iterator_dereferencing< Container1 >::type, \
        typename _tt::result_of_dereferencing< InputIterator2 & >::type \
    >::type

    template<typename Container1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            GTFO_COMMON_TYPE_VALUE &,
            typename _tt::result_of_addition
            <
                GTFO_COMMON_TYPE_VALUE &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_container_iterator_dereferencing< Container1 >::type,
                    typename _tt::result_of_dereferencing< InputIterator2 & >::type
                >::type
            >::type
        >::value,
        GTFO_COMMON_TYPE_VALUE
    >::type
    inner_product(Container1 &&  container1,
                  InputIterator2 it_begin_2)
    {
        return ::std::inner_product(begin(container1), end(container1), it_begin_2,
                                    GTFO_COMMON_TYPE_VALUE());
    }

#undef GTFO_COMMON_TYPE_VALUE

#define GTFO_COMMON_TYPE_VALUE \
    typename _tt::common_type_2 \
    < \
        typename _tt::result_of_dereferencing< InputIterator1 & >::type, \
        typename _tt::result_of_container_iterator_dereferencing< Container2 >::type \
    >::type

    template<typename InputIterator1, typename Container2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_container_castable_to_its_iterator<Container2>::value
        &&
        _tt::is_assignable
        <
            GTFO_COMMON_TYPE_VALUE &,
            typename _tt::result_of_addition
            <
                GTFO_COMMON_TYPE_VALUE &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_dereferencing< InputIterator1 & >::type,
                    typename _tt::result_of_container_iterator_dereferencing< Container2 >::type
                >::type
            >::type
        >::value,
        GTFO_COMMON_TYPE_VALUE
    >::type
    inner_product(InputIterator1 it_begin_1,
                  InputIterator1 it_end_1,
                  Container2 &&  container2)
    {
        return ::std::inner_product(it_begin_1, it_end_1, begin(container2),
                                    GTFO_COMMON_TYPE_VALUE());
    }

#undef GTFO_COMMON_TYPE_VALUE

#define GTFO_COMMON_TYPE_VALUE \
    typename _tt::common_type_2 \
    < \
        typename _tt::result_of_container_iterator_dereferencing< Container1 >::type, \
        typename _tt::result_of_container_iterator_dereferencing< Container2 >::type \
    >::type

    template<typename Container1, typename Container2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_container_castable_to_its_iterator<Container2>::value
        &&
        _tt::is_assignable
        <
            GTFO_COMMON_TYPE_VALUE &,
            typename _tt::result_of_addition
            <
                GTFO_COMMON_TYPE_VALUE &,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_container_iterator_dereferencing< Container1 >::type,
                    typename _tt::result_of_container_iterator_dereferencing< Container2 >::type
                >::type
            >::type
        >::value,
        GTFO_COMMON_TYPE_VALUE
    >::type
    inner_product(Container1 && container1,
                  Container2 && container2)
    {
        return ::std::inner_product(begin(container1), end(container1), begin(container2),
                                    GTFO_COMMON_TYPE_VALUE());
    }

#undef GTFO_COMMON_TYPE_VALUE

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP

