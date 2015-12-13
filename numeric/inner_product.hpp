#ifndef GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP

#include <numeric>
#include "_impl/type_traits/common_type_2.hpp"
#include "_impl/type_traits/result_of_dereferencing.hpp"
#include "_impl/type_traits/is_assignable.hpp"
#include "_impl/type_traits/result_of_addition.hpp"
#include "_impl/type_traits/result_of_multiplication.hpp"
#include "_impl/type_traits/result_of_fun2.hpp"

namespace gtfo
{
#define GTFO_COMMON_TYPE_VALUE \
    typename _tt::common_type_2 \
    < \
        typename _tt::result_of_dereferencing<InputIterator1 &>::type, \
        typename _tt::result_of_dereferencing<InputIterator2 &>::type \
    >::type
#define GTFO_COMMON_TYPE_VALUE_REF \
    typename _tt::add_lvalue_reference<GTFO_COMMON_TYPE_VALUE>::type

    template<typename InputIterator1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            GTFO_COMMON_TYPE_VALUE_REF,
            typename _tt::result_of_addition
            <
                GTFO_COMMON_TYPE_VALUE_REF,
                typename _tt::result_of_multiplication
                <
                    typename _tt::result_of_dereferencing<InputIterator1 &>::type,
                    typename _tt::result_of_dereferencing<InputIterator2 &>::type
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

    template<typename InputIterator1, typename InputIterator2,
             typename BinaryOperation1, typename BinaryOperation2>
    inline
    typename _tt::enable_if
    <
        _tt::is_assignable
        <
            GTFO_COMMON_TYPE_VALUE_REF,
            typename _tt::result_of_fun2
            <
                BinaryOperation1,
                GTFO_COMMON_TYPE_VALUE_REF,
                typename _tt::result_of_fun2
                <
                    BinaryOperation2,
                    typename _tt::result_of_dereferencing<InputIterator1 &>::type,
                    typename _tt::result_of_dereferencing<InputIterator2 &>::type
                >::type
            >::type
        >::value,
        GTFO_COMMON_TYPE_VALUE
    >::type
    inner_product(InputIterator1   it_begin_1,
                  InputIterator1   it_end_1,
                  InputIterator2   it_begin_2,
                  BinaryOperation1 op1,
                  BinaryOperation2 op2)
    {
        return ::std::inner_product(it_begin_1, it_end_1, it_begin_2,
                                    GTFO_COMMON_TYPE_VALUE(),
                                    op1, op2);
    }

// TODO : implement the following (all of 'em? srsly?)
// inner_product( Container1,                     InputIterator2, Value           )
// inner_product( Container1,                     InputIterator2, Value, Op1, Op2 )
// inner_product( InputIterator1, InputIterator1, Container2,     Value           )
// inner_product( InputIterator1, InputIterator1, Container2,     Value, Op1, Op2 )
// inner_product( Container1,                     Container2,     Value           )
// inner_product( Container1,                     Container2,     Value, Op1, Op2 )
// inner_product( Container1,                     InputIterator2,                 )
// inner_product( Container1,                     InputIterator2,        Op1, Op2 )
// inner_product( InputIterator1, InputIterator1, Container2,                     )
// inner_product( InputIterator1, InputIterator1, Container2,            Op1, Op2 )
// inner_product( Container1,                     Container2,                     )
// inner_product( Container1,                     Container2,            Op1, Op2 )

#undef GTFO_COMMON_TYPE_VALUE_REF
#undef GTFO_COMMON_TYPE_VALUE
}
#endif // GTFO_FILE_INCLUDED_NUMERIC_INNER_PRODUCT_HPP

