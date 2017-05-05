#ifndef GTFO_FILE_INCLUDED_ALGORITHM_IS_PERMUTATION_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_IS_PERMUTATION_HPP

/*
 * Defines the following overloads:
 *     is_permutation(ForwardIterator1, ForwardIterator1, ForwardIterator2);
 *     is_permutation(ForwardIterator1, ForwardIterator1, ForwardIterator2, BinaryPredicate);
 *     is_permutation(ForwardIterator1, ForwardIterator1, ForwardIterator2, ForwardIterator2); // C++14 only!
 *     is_permutation(ForwardIterator1, ForwardIterator1, ForwardIterator2, ForwardIterator2, BinaryPredicate); // C++14 only!
 *     is_permutation(ForwardIterator1, ForwardIterator1, Range2);
 *     is_permutation(ForwardIterator1, ForwardIterator1, Range2,           BinaryPredicate);
 *     is_permutation(Range1,           ForwardIterator2);
 *     is_permutation(Range1,           ForwardIterator2, BinaryPredicate);
 *     is_permutation(Range1,           ForwardIterator2, ForwardIterator2); // C++14 only!
 *     is_permutation(Range1,           ForwardIterator2, ForwardIterator2, BinaryPredicate); // C++14 only!
 *     is_permutation(Range1,           Range2);
 *     is_permutation(Range1,           Range2,           BinaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/is_range_castable_to_its_iterator.hpp"

#if (__cplusplus >= 201300L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
#endif

namespace gtfo
{

#define GTFO_RESULT_OF_IS_PERMUTATION(ForwardIterator1, ForwardIterator2) \
    typename _tt::enable_if \
    < \
        _tt::are_comparable_op_eq \
        < \
            typename _tt::result_of_dereferencing< ForwardIterator1 >::type, \
            typename _tt::result_of_dereferencing< ForwardIterator2 >::type \
        >::value, \
        bool \
    >::type

#define GTFO_RESULT_OF_IS_PERMUTATION_PRED(ForwardIterator1, ForwardIterator2, BinaryPredicate) \
    typename _tt::enable_if \
    < \
        _tt::are_comparable_pred \
        < \
            BinaryPredicate, \
            typename _tt::result_of_dereferencing< ForwardIterator1 >::type, \
            typename _tt::result_of_dereferencing< ForwardIterator2 >::type \
        >::value, \
        bool \
    >::type

    template<typename ForwardIterator1, typename ForwardIterator2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< ForwardIterator2 >::value,
        GTFO_RESULT_OF_IS_PERMUTATION(ForwardIterator1,
                                      ForwardIterator2)
    >::type
    is_permutation(ForwardIterator1    _it_begin_1,
                   ForwardIterator1    _it_end_1,
                   ForwardIterator2 && _it_begin_2)
    {
        return ::std::is_permutation(::gtfo::move(_it_begin_1),
                                     ::gtfo::move(_it_end_1),
                                     ::gtfo::forward<ForwardIterator2>(_it_begin_2));
    }

    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< ForwardIterator2 >::value,
        GTFO_RESULT_OF_IS_PERMUTATION_PRED(ForwardIterator1,
                                           ForwardIterator2,
                                           BinaryPredicate)
    >::type
    is_permutation(ForwardIterator1    _it_begin_1,
                   ForwardIterator1    _it_end_1,
                   ForwardIterator2 && _it_begin_2,
                   BinaryPredicate     _pred)
    {
        return ::std::is_permutation(::gtfo::move(_it_begin_1),
                                     ::gtfo::move(_it_end_1),
                                     ::gtfo::forward<ForwardIterator2>(_it_begin_2),
                                     ::gtfo::move(_pred));
    }

#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
    template<typename ForwardIterator1, typename ForwardIterator2>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION(ForwardIterator1,
                                  ForwardIterator2)
    is_permutation(ForwardIterator1 _it_begin_1,
                   ForwardIterator1 _it_end_1,
                   ForwardIterator2 _it_begin_2,
                   ForwardIterator2 _it_end_2)
    {
        return ::std::is_permutation(::gtfo::move(_it_begin_1),
                                     ::gtfo::move(_it_end_1),
                                     ::gtfo::move(_it_begin_2),
                                     ::gtfo::move(_it_end_2));
    }

    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    bool
    is_permutation(ForwardIterator1 _it_begin_1,
                   ForwardIterator1 _it_end_1,
                   ForwardIterator2 _it_begin_2,
                   ForwardIterator2 _it_end_2,
                   BinaryPredicate  _pred)
    {
        return ::std::is_permutation(::gtfo::move(_it_begin_1),
                                     ::gtfo::move(_it_end_1),
                                     ::gtfo::move(_it_begin_2),
                                     ::gtfo::move(_it_end_2),
                                     ::gtfo::move(_pred));
    }
#endif // GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM

    template<typename ForwardIterator1, typename Range2>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION(ForwardIterator1,
                                  typename _tt::iterator_of_range< Range2 >::type)
    is_permutation(ForwardIterator1 _it_begin_1,
                   ForwardIterator1 _it_end_1,
                   Range2 &&        _range_2)
    {
        return ::std::is_permutation(::gtfo::move(_it_begin_1),
                                     ::gtfo::move(_it_end_1),
                                     begin(_range_2)
#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
                ,                    end(_range_2)
#endif
                );
    }

    template<typename ForwardIterator1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION_PRED(ForwardIterator1,
                                       typename _tt::iterator_of_range< Range2 >::type,
                                       BinaryPredicate)
    is_permutation(ForwardIterator1 _it_begin_1,
                   ForwardIterator1 _it_end_1,
                   Range2 &&        _range_2,
                   BinaryPredicate  _pred)
    {
        return ::std::is_permutation(::gtfo::move(_it_begin_1),
                                     ::gtfo::move(_it_end_1),
                                     begin(_range_2),
#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
                                     end(_range_2),
#endif
                                     ::gtfo::move(_pred));
    }

    template<typename Range1, typename ForwardIterator2>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< ForwardIterator2 >::value,
        GTFO_RESULT_OF_IS_PERMUTATION(typename _tt::iterator_of_range< Range1 >::type,
                                      ForwardIterator2)
    >::type
    is_permutation(Range1 &&           _range_1,
                   ForwardIterator2 && _it_begin_2)
    {
        return ::std::is_permutation(begin(_range_1),
                                     end(_range_1),
                                     ::gtfo::forward<ForwardIterator2>(_it_begin_2));
    }

    template<typename Range1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    typename _tt::enable_if
    <
        !_tt::is_range_castable_to_its_iterator< ForwardIterator2 >::value,
        GTFO_RESULT_OF_IS_PERMUTATION_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                           ForwardIterator2,
                                           BinaryPredicate)
    >::type
    is_permutation(Range1 &&           _range_1,
                   ForwardIterator2 && _it_begin_2,
                   BinaryPredicate     _pred)
    {
        return ::std::is_permutation(begin(_range_1),
                                     end(_range_1),
                                     ::gtfo::forward<ForwardIterator2>(_it_begin_2),
                                     ::gtfo::move(_pred));
    }

#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
    template<typename Range1, typename ForwardIterator2>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION(typename _tt::iterator_of_range< Range1 >::type,
                                  ForwardIterator2)
    is_permutation(Range1 &&        _range_1,
                   ForwardIterator2 _it_begin_2,
                   ForwardIterator2 _it_end_2)
    {
        return ::std::is_permutation(begin(_range_1),
                                     end(_range_1),
                                     ::gtfo::move(_it_begin_2),
                                     ::gtfo::move(_it_end_2));
    }

    template<typename Range1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                       ForwardIterator2,
                                       BinaryPredicate)
    is_permutation(Range1 &&        _range_1,
                   ForwardIterator2 _it_begin_2,
                   ForwardIterator2 _it_end_2,
                   BinaryPredicate  _pred)
    {
        return ::std::is_permutation(begin(_range_1),
                                     end(_range_1),
                                     ::gtfo::move(_it_begin_2),
                                     ::gtfo::move(_it_end_2),
                                     ::gtfo::move(_pred));
    }
#endif // GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM


    template<typename Range1, typename Range2>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION(typename _tt::iterator_of_range< Range1 >::type,
                                  typename _tt::iterator_of_range< Range2 >::type)
    is_permutation(Range1 && _range_1,
                   Range2 && _range_2)
    {
        return ::std::is_permutation(begin(_range_1),
                                     end(_range_1),
                                     begin(_range_2)
#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
                ,                    end(_range_2)
#endif
                );
    }

    template<typename Range1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_IS_PERMUTATION_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                       typename _tt::iterator_of_range< Range2 >::type,
                                       BinaryPredicate)
    is_permutation(Range1 &&       _range_1,
                   Range2 &&       _range_2,
                   BinaryPredicate _pred)
    {
        return ::std::is_permutation(begin(_range_1),
                                     end(_range_1),
                                     begin(_range_2),
#ifdef GTFO_USE_CPP14_IS_PERMUTATION_ALGORITHM
                                     end(_range_2),
#endif
                                     ::gtfo::move(_pred));
    }

#undef GTFO_RESULT_OF_IS_PERMUTATION_PRED
#undef GTFO_RESULT_OF_IS_PERMUTATION

}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_IS_PERMUTATION_HPP
