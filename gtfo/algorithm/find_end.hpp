#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FIND_END_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FIND_END_HPP

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_equality_comparable.hpp"
#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/result_of_fun2.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_FIND_END(ForwardIterator1, ForwardIterator2) \
    typename _tt::enable_if                                             \
    <                                                                      \
        _tt::is_equality_comparable                                          \
        <                                                                     \
            typename _tt::result_of_dereferencing< ForwardIterator1 >::type,  \
            typename _tt::result_of_dereferencing< ForwardIterator2 >::type   \
        >::value,                                                           \
        ForwardIterator1                                                 \
    >::type

#define GTFO_RESULT_OF_FIND_END_PRED(ForwardIterator1, ForwardIterator2, BinaryPredicate) \
    typename _tt::enable_if                                                               \
    <                                                                                     \
        _tt::can_be_used_in_boolean_context                                               \
        <                                                                                \
            typename _tt::result_of_fun2                                                \
            <                                                                          \
                BinaryPredicate,                                                      \
                typename _tt::result_of_dereferencing< ForwardIterator1 >::type,     \
                typename _tt::result_of_dereferencing< ForwardIterator2 >::type    \
            >::type                                                              \
        >::value,                                                             \
        ForwardIterator1                                                  \
    >::type

    template<typename ForwardIterator1, typename ForwardIterator2>
    inline
    GTFO_RESULT_OF_FIND_END(ForwardIterator1,
                            ForwardIterator2)
    find_end(ForwardIterator1 it_begin_1,
             ForwardIterator1 it_end_1,
             ForwardIterator2 it_begin_2,
             ForwardIterator2 it_end_2)
    {
        return ::std::find_end(::gtfo::move(it_begin_1),
                               ::gtfo::move(it_end_1),
                               ::gtfo::move(it_begin_2),
                               ::gtfo::move(it_end_2));
    }

    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_END_PRED(ForwardIterator1,
                                 ForwardIterator2,
                                 BinaryPredicate)
    find_end(ForwardIterator1 it_begin_1,
             ForwardIterator1 it_end_1,
             ForwardIterator2 it_begin_2,
             ForwardIterator2 it_end_2,
             BinaryPredicate  pred)
    {
        return ::std::find_end(::gtfo::move(it_begin_1),
                               ::gtfo::move(it_end_1),
                               ::gtfo::move(it_begin_2),
                               ::gtfo::move(it_end_2),
                               ::gtfo::move(pred));
    }

    template<typename ForwardIterator1, typename Range2>
    inline
    GTFO_RESULT_OF_FIND_END(ForwardIterator1,
                            typename _tt::iterator_of_range< Range2 >::type)
    find_end(ForwardIterator1 it_begin_1,
             ForwardIterator1 it_end_1,
             Range2 &&        range2)
    {
        return ::std::find_end(::gtfo::move(it_begin_1),
                               ::gtfo::move(it_end_1),
                               ::gtfo::begin(range2),
                               ::gtfo::end(range2));
    }

    template<typename ForwardIterator1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_END_PRED(ForwardIterator1,
                                 typename _tt::iterator_of_range< Range2 >::type,
                                 BinaryPredicate)
    find_end(ForwardIterator1 it_begin_1,
             ForwardIterator1 it_end_1,
             Range2 &&        range2,
             BinaryPredicate  pred)
    {
        return ::std::find_end(::gtfo::move(it_begin_1),
                               ::gtfo::move(it_end_1),
                               ::gtfo::begin(range2),
                               ::gtfo::end(range2),
                               ::gtfo::move(pred));
    }

    template<typename Range1, typename ForwardIterator2>
    inline
    GTFO_RESULT_OF_FIND_END(typename _tt::iterator_of_range< Range1 >::type,
                            ForwardIterator2)
    find_end(Range1 &&        range1,
             ForwardIterator2 it_begin_2,
             ForwardIterator2 it_end_2)
    {
        return ::std::find_end(::gtfo::begin(range1),
                               ::gtfo::end(range1),
                               ::gtfo::move(it_begin_2),
                               ::gtfo::move(it_end_2));
    }

    template<typename Range1, typename ForwardIterator2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_END_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                 ForwardIterator2,
                                 BinaryPredicate)
    find_end(Range1 &&        range1,
             ForwardIterator2 it_begin_2,
             ForwardIterator2 it_end_2,
             BinaryPredicate  pred)
    {
        return ::std::find_end(::gtfo::begin(range1),
                               ::gtfo::end(range1),
                               ::gtfo::move(it_begin_2),
                               ::gtfo::move(it_end_2),
                               ::gtfo::move(pred));
    }

    template<typename Range1, typename Range2>
    inline
    GTFO_RESULT_OF_FIND_END(typename _tt::iterator_of_range< Range1 >::type,
                            typename _tt::iterator_of_range< Range2 >::type)
    find_end(Range1 && range1,
             Range2 && range2)
    {
        return ::std::find_end(::gtfo::begin(range1),
                               ::gtfo::end(range1),
                               ::gtfo::begin(range2),
                               ::gtfo::end(range2));
    }

    template<typename Range1, typename Range2, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_FIND_END_PRED(typename _tt::iterator_of_range< Range1 >::type,
                                 typename _tt::iterator_of_range< Range2 >::type,
                                 BinaryPredicate)
    find_end(Range1 &&       range1,
             Range2 &&       range2,
             BinaryPredicate pred)
    {
        return ::std::find_end(::gtfo::begin(range1),
                               ::gtfo::end(range1),
                               ::gtfo::begin(range2),
                               ::gtfo::end(range2),
                               ::gtfo::move(pred));
    }

#undef GTFO_RESULT_OF_FIND_END_PRED
#undef GTFO_RESULT_OF_FIND_END

}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FIND_END_HPP
