#ifndef GTFO_FILE_INCLUDED_ALGORITHM_EQUAL_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_EQUAL_HPP

/*
 * Defines the following overloads:
 *     equal(InputIterator1, InputIterator1, InputIterator2);
 *     equal(InputIterator1, InputIterator1, InputIterator2, BinaryPredicate);
 *     equal(InputIterator1, InputIterator1, InputIterator2, InputIterator2); // C++14 only!
 *     equal(InputIterator1, InputIterator1, InputIterator2, InputIterator2, BinaryPredicate); // C++14 only!
 *     equal(InputIterator1, InputIterator1, Range2);
 *     equal(InputIterator1, InputIterator1, Range2,         BinaryPredicate);
 *     equal(Range1,         InputIterator2);
 *     equal(Range1,         InputIterator2, BinaryPredicate);
 *     equal(Range1,         InputIterator2, InputIterator2); // C++14 only!
 *     equal(Range1,         InputIterator2, InputIterator2, BinaryPredicate); // C++14 only!
 *     equal(Range1,         Range2);
 *     equal(Range1,         Range2,         BinaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/is_range_castable_to_its_iterator.hpp"

#if (__cplusplus >= 201300L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define GTFO_USE_CPP14_EQUAL_ALGORITHM
#endif

namespace gtfo
{

#define GTFO_RESULT_OF_EQUAL(InputIterator1, InputIterator2) \
    typename _tt::enable_if \
    < \
        _tt::are_comparable_op_eq \
        < \
            typename _tt::result_of_dereferencing< InputIterator1 >::type, \
            typename _tt::result_of_dereferencing< InputIterator2 >::type \
        >::value, \
        bool \
    >::type

#define GTFO_RESULT_OF_EQUAL_PRED(InputIterator1, InputIterator2, BinaryPredicate) \
    typename _tt::enable_if \
    < \
        _tt::are_comparable_pred \
        < \
            BinaryPredicate, \
            typename _tt::result_of_dereferencing< InputIterator1 >::type, \
            typename _tt::result_of_dereferencing< InputIterator2 >::type \
        >::value, \
        bool \
    >::type

    template<typename InputIterator1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_EQUAL(InputIterator1,
                             InputIterator2)
    >::type
    equal(InputIterator1    _it_begin_1,
          InputIterator1    _it_end_1,
          InputIterator2 && _it_begin_2)
    {
        return ::std::equal(::gtfo::move(_it_begin_1),
                            ::gtfo::move(_it_end_1),
                            ::gtfo::forward<InputIterator2>(_it_begin_2));
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_EQUAL_PRED(InputIterator1,
                                  InputIterator2,
                                  BinaryPredicate)
    >::type
    equal(InputIterator1    _it_begin_1,
          InputIterator1    _it_end_1,
          InputIterator2 && _it_begin_2,
          BinaryPredicate   _pred)
    {
        return ::std::equal(::gtfo::move(_it_begin_1),
                            ::gtfo::move(_it_end_1),
                            ::gtfo::forward<InputIterator2>(_it_begin_2),
                            ::gtfo::move(_pred));
    }

#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
    template<typename InputIterator1, typename InputIterator2>
    inline
    GTFO_RESULT_OF_EQUAL(InputIterator1,
                         InputIterator2)
    equal(InputIterator1 _it_begin_1,
          InputIterator1 _it_end_1,
          InputIterator2 _it_begin_2,
          InputIterator2 _it_end_2)
    {
        return ::std::equal(::gtfo::move(_it_begin_1),
                            ::gtfo::move(_it_end_1),
                            ::gtfo::move(_it_begin_2),
                            ::gtfo::move(_it_end_2));
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    inline
    bool
    equal(InputIterator1  _it_begin_1,
          InputIterator1  _it_end_1,
          InputIterator2  _it_begin_2,
          InputIterator2  _it_end_2,
          BinaryPredicate _pred)
    {
        return ::std::equal(::gtfo::move(_it_begin_1),
                            ::gtfo::move(_it_end_1),
                            ::gtfo::move(_it_begin_2),
                            ::gtfo::move(_it_end_2),
                            ::gtfo::move(_pred));
    }
#endif // GTFO_USE_CPP14_EQUAL_ALGORITHM

    template<typename InputIterator1, typename Range2>
    inline
    GTFO_RESULT_OF_EQUAL(InputIterator1,
                         typename _tt::iterator_of_range< Range2 >::type)
    equal(InputIterator1 _it_begin_1,
          InputIterator1 _it_end_1,
          Range2 &&      _range_2)
    {
        return ::std::equal(::gtfo::move(_it_begin_1),
                            ::gtfo::move(_it_end_1),
                            begin(_range_2)
#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
                ,           end(_range_2)
#endif
                );
    }

    template<typename InputIterator1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_EQUAL_PRED(InputIterator1,
                              typename _tt::iterator_of_range< Range2 >::type,
                              BinaryPredicate)
    equal(InputIterator1  _it_begin_1,
          InputIterator1  _it_end_1,
          Range2 &&       _range_2,
          BinaryPredicate _pred)
    {
        return ::std::equal(::gtfo::move(_it_begin_1),
                            ::gtfo::move(_it_end_1),
                            begin(_range_2),
#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
                            end(_range_2),
#endif
                            ::gtfo::move(_pred));
    }

    template<typename Range1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_EQUAL(typename _tt::iterator_of_range< Range1 >::type,
                             InputIterator2)
    >::type
    equal(Range1 &&         _range_1,
          InputIterator2 && _it_begin_2)
    {
        return ::std::equal(begin(_range_1),
                            end(_range_1),
                            ::gtfo::forward<InputIterator2>(_it_begin_2));
    }

    template<typename Range1, typename InputIterator2, typename BinaryPredicate>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_EQUAL_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                  InputIterator2,
                                  BinaryPredicate)
    >::type
    equal(Range1 &&         _range_1,
          InputIterator2 && _it_begin_2,
          BinaryPredicate   _pred)
    {
        return ::std::equal(begin(_range_1),
                            end(_range_1),
                            ::gtfo::forward<InputIterator2>(_it_begin_2),
                            ::gtfo::move(_pred));
    }

#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
    template<typename Range1, typename InputIterator2>
    inline
    GTFO_RESULT_OF_EQUAL(typename _tt::iterator_of_range< Range1 >::type,
                         InputIterator2)
    equal(Range1 &&      _range_1,
          InputIterator2 _it_begin_2,
          InputIterator2 _it_end_2)
    {
        return ::std::equal(begin(_range_1),
                            end(_range_1),
                            ::gtfo::move(_it_begin_2),
                            ::gtfo::move(_it_end_2));
    }

    template<typename Range1, typename InputIterator2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_EQUAL_PRED(typename _tt::iterator_of_range< Range1 >::type,
                              InputIterator2,
                              BinaryPredicate)
    equal(Range1 &&       _range_1,
          InputIterator2  _it_begin_2,
          InputIterator2  _it_end_2,
          BinaryPredicate _pred)
    {
        return ::std::equal(begin(_range_1),
                            end(_range_1),
                            ::gtfo::move(_it_begin_2),
                            ::gtfo::move(_it_end_2),
                            ::gtfo::move(_pred));
    }
#endif // GTFO_USE_CPP14_EQUAL_ALGORITHM


    template<typename Range1, typename Range2>
    inline
    GTFO_RESULT_OF_EQUAL(typename _tt::iterator_of_range< Range1 >::type,
                         typename _tt::iterator_of_range< Range2 >::type)
    equal(Range1 && _range_1,
          Range2 && _range_2)
    {
        return ::std::equal(begin(_range_1),
                            end(_range_1),
                            begin(_range_2)
#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
                ,           end(_range_2)
#endif
                );
    }

    template<typename Range1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_EQUAL_PRED(typename _tt::iterator_of_range< Range1 >::type,
                              typename _tt::iterator_of_range< Range2 >::type,
                              BinaryPredicate)
    equal(Range1 &&       _range_1,
          Range2 &&       _range_2,
          BinaryPredicate _pred)
    {
        return ::std::equal(begin(_range_1),
                            end(_range_1),
                            begin(_range_2),
#ifdef GTFO_USE_CPP14_EQUAL_ALGORITHM
                            end(_range_2),
#endif
                            ::gtfo::move(_pred));
    }

#undef GTFO_RESULT_OF_EQUAL_PRED
#undef GTFO_RESULT_OF_EQUAL

}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_EQUAL_HPP
