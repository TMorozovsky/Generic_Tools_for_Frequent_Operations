#ifndef GTFO_FILE_INCLUDED_ALGORITHM_MISMATCH_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_MISMATCH_HPP

/*
 * Defines the following overloads:
 *     mismatch(InputIterator1, InputIterator1, InputIterator2);
 *     mismatch(InputIterator1, InputIterator1, InputIterator2,  BinaryPredicate);
 *     mismatch(InputIterator1, InputIterator1, InputIterator2,  InputIterator2); // C++14 only!
 *     mismatch(InputIterator1, InputIterator1, InputIterator2,  InputIterator2,  BinaryPredicate); // C++14 only!
 *     mismatch(InputIterator1, InputIterator1, Range2);
 *     mismatch(InputIterator1, InputIterator1, Range2,          BinaryPredicate);
 *     mismatch(Range1,         InputIterator2);
 *     mismatch(Range1,         InputIterator2, BinaryPredicate);
 *     mismatch(Range1,         InputIterator2, InputIterator2); // C++14 only!
 *     mismatch(Range1,         InputIterator2, InputIterator2,  BinaryPredicate); // C++14 only!
 *     mismatch(Range1,         Range2);
 *     mismatch(Range1,         Range2,         BinaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/is_range_castable_to_its_iterator.hpp"

#if (__cplusplus >= 201300L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define GTFO_USE_CPP14_MISMATCH_ALGORITHM
#endif

namespace gtfo
{

#define GTFO_RESULT_OF_MISMATCH(InputIterator1, InputIterator2) \
    typename _tt::enable_if                                          \
    <                                                                    \
        _tt::are_comparable_op_eq                                          \
        <                                                                   \
            typename _tt::result_of_dereferencing< InputIterator1 >::type,  \
            typename _tt::result_of_dereferencing< InputIterator2 >::type   \
        >::value,                                                          \
        ::std::pair< InputIterator1, InputIterator2 >                   \
    >::type

#define GTFO_RESULT_OF_MISMATCH_PRED(InputIterator1, InputIterator2, BinaryPredicate) \
    typename _tt::enable_if                                                           \
    <                                                                                \
        _tt::are_comparable_pred                                                    \
        <                                                                          \
            BinaryPredicate,                                                      \
            typename _tt::result_of_dereferencing< InputIterator1 >::type,       \
            typename _tt::result_of_dereferencing< InputIterator2 >::type      \
        >::value,                                                            \
        ::std::pair< InputIterator1, InputIterator2 >                     \
    >::type

    template<typename InputIterator1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_MISMATCH(InputIterator1,
                                InputIterator2)
    >::type
    mismatch(InputIterator1    it_begin_1,
             InputIterator1    it_end_1,
             InputIterator2 && it_begin_2)
    {
        return ::std::mismatch(_utils::move(it_begin_1),
                               _utils::move(it_end_1),
                               _utils::forward<InputIterator2>(it_begin_2));
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_MISMATCH_PRED(InputIterator1,
                                     InputIterator2,
                                     BinaryPredicate)
    >::type
    mismatch(InputIterator1    it_begin_1,
             InputIterator1    it_end_1,
             InputIterator2 && it_begin_2,
             BinaryPredicate   pred)
    {
        return ::std::mismatch(_utils::move(it_begin_1),
                               _utils::move(it_end_1),
                               _utils::forward<InputIterator2>(it_begin_2),
                               _utils::move(pred));
    }

#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
    template<typename InputIterator1, typename InputIterator2>
    inline
    GTFO_RESULT_OF_MISMATCH(InputIterator1,
                            InputIterator2)
    mismatch(InputIterator1 it_begin_1,
             InputIterator1 it_end_1,
             InputIterator2 it_begin_2,
             InputIterator2 it_end_2)
    {
        return ::std::mismatch(_utils::move(it_begin_1),
                               _utils::move(it_end_1),
                               _utils::move(it_begin_2),
                               _utils::move(it_end_2));
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    inline
    ::std::pair< InputIterator1, InputIterator2 >
    mismatch(InputIterator1  it_begin_1,
             InputIterator1  it_end_1,
             InputIterator2  it_begin_2,
             InputIterator2  it_end_2,
             BinaryPredicate pred)
    {
        return ::std::mismatch(_utils::move(it_begin_1),
                               _utils::move(it_end_1),
                               _utils::move(it_begin_2),
                               _utils::move(it_end_2),
                               _utils::move(pred));
    }
#endif // GTFO_USE_CPP14_MISMATCH_ALGORITHM

    template<typename InputIterator1, typename Range2>
    inline
    GTFO_RESULT_OF_MISMATCH(InputIterator1,
                            typename _tt::iterator_of_range< Range2 >::type)
    mismatch(InputIterator1 it_begin_1,
             InputIterator1 it_end_1,
             Range2 &&      range_2)
    {
        return ::std::mismatch(_utils::move(it_begin_1),
                               _utils::move(it_end_1),
                               begin(range_2)
#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
                               , end(range_2)
#endif
                               );
    }

    template<typename InputIterator1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_MISMATCH_PRED(InputIterator1,
                                 typename _tt::iterator_of_range< Range2 >::type,
                                 BinaryPredicate)
    mismatch(InputIterator1  it_begin_1,
             InputIterator1  it_end_1,
             Range2 &&       range_2,
             BinaryPredicate pred)
    {
        return ::std::mismatch(_utils::move(it_begin_1),
                               _utils::move(it_end_1),
                               begin(range_2),
#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
                               end(range_2),
#endif
                               _utils::move(pred));
    }

    template<typename Range1, typename InputIterator2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_MISMATCH(typename _tt::iterator_of_range< Range1 >::type,
                                InputIterator2)
    >::type
    mismatch(Range1 &&         range_1,
             InputIterator2 && it_begin_2)
    {
        return ::std::mismatch(begin(range_1),
                               end(range_1),
                               _utils::forward<InputIterator2>(it_begin_2)
                               );
    }

    template<typename Range1, typename InputIterator2, typename BinaryPredicate>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< InputIterator2 >::value,
        GTFO_RESULT_OF_MISMATCH_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                     InputIterator2,
                                     BinaryPredicate)
    >::type
    mismatch(Range1 &&         range_1,
             InputIterator2 && it_begin_2,
             BinaryPredicate   pred)
    {
        return ::std::mismatch(begin(range_1),
                               end(range_1),
                               _utils::forward<InputIterator2>(it_begin_2),
                               _utils::move(pred)
                               );
    }

#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
    template<typename Range1, typename InputIterator2>
    inline
    GTFO_RESULT_OF_MISMATCH(typename _tt::iterator_of_range< Range1 >::type,
                            InputIterator2)
    mismatch(Range1 &&      range_1,
             InputIterator2 it_begin_2,
             InputIterator2 it_end_2)
    {
        return ::std::mismatch(begin(range_1),
                               end(range_1),
                               _utils::move(it_begin_2),
                               _utils::move(it_end_2)
                               );
    }

    template<typename Range1, typename InputIterator2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_MISMATCH_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                 InputIterator2,
                                 BinaryPredicate)
    mismatch(Range1 &&       range_1,
             InputIterator2  it_begin_2,
             InputIterator2  it_end_2,
             BinaryPredicate pred)
    {
        return ::std::mismatch(begin(range_1),
                               end(range_1),
                               _utils::move(it_begin_2),
                               _utils::move(it_end_2),
                               _utils::move(pred)
                               );
    }
#endif // GTFO_USE_CPP14_MISMATCH_ALGORITHM

    template<typename Range1, typename Range2>
    inline
    GTFO_RESULT_OF_MISMATCH(typename _tt::iterator_of_range< Range1 >::type,
                            typename _tt::iterator_of_range< Range2 >::type)
    mismatch(Range1 && range_1,
             Range2 && range_2)
    {
        return ::std::mismatch(begin(range_1),
                               end(range_1),
                               begin(range_2)
#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
                               , end(range_2)
#endif
                               );
    }

    template<typename Range1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_MISMATCH_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                 typename _tt::iterator_of_range< Range2 >::type,
                                 BinaryPredicate)
    mismatch(Range1 &&       range_1,
             Range2 &&       range_2,
             BinaryPredicate pred)
    {
        return ::std::mismatch(begin(range_1),
                               end(range_1),
                               begin(range_2),
#ifdef GTFO_USE_CPP14_MISMATCH_ALGORITHM
                               end(range_2),
#endif
                               _utils::move(pred));
    }

#undef GTFO_RESULT_OF_MISMATCH_PRED
#undef GTFO_RESULT_OF_MISMATCH

}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_MISMATCH_HPP
