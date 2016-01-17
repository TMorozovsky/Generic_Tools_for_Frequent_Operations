#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_FIRST_OF_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_FIRST_OF_HPP

/*
 * Defines the following overloads:
 *     find_first_of(ForwardIterator1, ForwardIterator1, ForwardIterator2, ForwardIterator2);
 *     find_first_of(ForwardIterator1, ForwardIterator1, ForwardIterator2, ForwardIterator2, BinaryPredicate);
 *     find_first_of(ForwardIterator1, ForwardIterator1, Range2);
 *     find_first_of(ForwardIterator1, ForwardIterator1, Range2,           BinaryPredicate);
 *     find_first_of(Range1,           ForwardIterator2, ForwardIterator2);
 *     find_first_of(Range1,           ForwardIterator2, ForwardIterator2, BinaryPredicate);
 *     find_first_of(Range1,           Range2);
 *     find_first_of(Range1,           Range2,           BinaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_FIND_FIRST_OF(ForwardIterator1, ForwardIterator2) \
    typename _tt::enable_if                                                \
    <                                                                        \
        _tt::are_comparable_op_eq                                             \
        <                                                                     \
            typename _tt::result_of_dereferencing< ForwardIterator2 >::type,  \
            typename _tt::result_of_dereferencing< ForwardIterator1 >::type   \
        >::value,                                                           \
        ForwardIterator1                                                 \
    >::type

#define GTFO_RESULT_OF_FIND_FIRST_OF_PRED(ForwardIterator1, ForwardIterator2, BinaryPredicate) \
    typename _tt::enable_if                                                                    \
    <                                                                                         \
        _tt::are_comparable_pred                                                            \
        <                                                                                 \
            BinaryPredicate,                                                           \
            typename _tt::result_of_dereferencing< ForwardIterator2 >::type,        \
            typename _tt::result_of_dereferencing< ForwardIterator1 >::type       \
        >::value,                                                              \
        ForwardIterator1                                                  \
    >::type

    template<typename ForwardIterator1, typename ForwardIterator2>
    inline
    GTFO_RESULT_OF_FIND_FIRST_OF(ForwardIterator1,
                                 ForwardIterator2)
    find_first_of(ForwardIterator1 it_begin_1,
                  ForwardIterator1 it_end_1,
                  ForwardIterator2 it_begin_2,
                  ForwardIterator2 it_end_2)
    {
        return ::std::find_first_of(_utils::move(it_begin_1),
                                    _utils::move(it_end_1),
                                    _utils::move(it_begin_2),
                                    _utils::move(it_end_2));
    }

    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    ForwardIterator1
    find_first_of(ForwardIterator1 it_begin_1,
                  ForwardIterator1 it_end_1,
                  ForwardIterator2 it_begin_2,
                  ForwardIterator2 it_end_2,
                  BinaryPredicate  pred)
    {
        return ::std::find_first_of(_utils::move(it_begin_1),
                                    _utils::move(it_end_1),
                                    _utils::move(it_begin_2),
                                    _utils::move(it_end_2),
                                    _utils::move(pred));
    }

    template<typename ForwardIterator1, typename Range2>
    inline
    GTFO_RESULT_OF_FIND_FIRST_OF(ForwardIterator1,
                                 typename _tt::iterator_of_range< Range2 >::type)
    find_first_of(ForwardIterator1 it_begin_1,
                  ForwardIterator1 it_end_1,
                  Range2 &&        range2)
    {
        return ::std::find_first_of(_utils::move(it_begin_1),
                                    _utils::move(it_end_1),
                                    begin(range2),
                                    end(range2));
    }

    template<typename ForwardIterator1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_FIRST_OF_PRED(ForwardIterator1,
                                      typename _tt::iterator_of_range< Range2 >::type,
                                      BinaryPredicate)
    find_first_of(ForwardIterator1 it_begin_1,
                  ForwardIterator1 it_end_1,
                  Range2 &&        range2,
                  BinaryPredicate  pred)
    {
        return ::std::find_first_of(_utils::move(it_begin_1),
                                    _utils::move(it_end_1),
                                    begin(range2),
                                    end(range2),
                                    _utils::move(pred));
    }

    template<typename Range1, typename ForwardIterator2>
    inline
    GTFO_RESULT_OF_FIND_FIRST_OF(typename _tt::iterator_of_range< Range1 >::type,
                                 ForwardIterator2)
    find_first_of(Range1 &&        range1,
                  ForwardIterator2 it_begin_2,
                  ForwardIterator2 it_end_2)
    {
        return ::std::find_first_of(begin(range1),
                                    end(range1),
                                    _utils::move(it_begin_2),
                                    _utils::move(it_end_2));
    }

    template<typename Range1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_FIRST_OF_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                      ForwardIterator2,
                                      BinaryPredicate)
    find_first_of(Range1 &&        range1,
                  ForwardIterator2 it_begin_2,
                  ForwardIterator2 it_end_2,
                  BinaryPredicate  pred)
    {
        return ::std::find_first_of(begin(range1),
                                    end(range1),
                                    _utils::move(it_begin_2),
                                    _utils::move(it_end_2),
                                    _utils::move(pred));
    }

    template<typename Range1, typename Range2>
    inline
    typename _tt::iterator_of_range< Range1 >::type
    find_first_of(Range1 && range1,
                  Range2 && range2)
    {
        return ::std::find_first_of(begin(range1),
                                    end(range1),
                                    begin(range2),
                                    end(range2));
    }

    template<typename Range1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_FIRST_OF_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                      typename _tt::iterator_of_range< Range2 >::type,
                                      BinaryPredicate)
    find_first_of(Range1 &&       range1,
                  Range2 &&       range2,
                  BinaryPredicate pred)
    {
        return ::std::find_first_of(begin(range1),
                                    end(range1),
                                    begin(range2),
                                    end(range2),
                                    _utils::move(pred));
    }

#undef GTFO_RESULT_OF_FIND_FIRST_OF_PRED
#undef GTFO_RESULT_OF_FIND_FIRST_OF

}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_FIRST_OF_HPP
