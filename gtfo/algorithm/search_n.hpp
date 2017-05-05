#ifndef GTFO_FILE_INCLUDED_ALGORITHM_SEARCH_N_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_SEARCH_N_HPP

/*
 * Defines the following overloads:
 *     search_n(ForwardIterator, ForwardIterator, Size,          const Value &);
 *     search_n(ForwardIterator, ForwardIterator, Size,          const Value &, BinaryPredicate);
 *     search_n(Range,           Size,            const Value &);
 *     search_n(Range,           Size,            const Value &, BinaryPredicate);
 */

#include <algorithm>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_SEARCH_N(ForwardIterator, Value) \
    typename _tt::enable_if \
    < \
        _tt::are_comparable_op_eq \
        < \
            typename _tt::result_of_dereferencing< ForwardIterator >::type, \
            Value \
        >::value, \
        ForwardIterator \
    >::type

#define GTFO_RESULT_OF_SEARCH_N_PRED(ForwardIterator, Value, BinaryPredicate) \
    typename _tt::enable_if \
    < \
        _tt::are_comparable_pred \
        < \
            BinaryPredicate, \
            typename _tt::result_of_dereferencing< ForwardIterator >::type, \
            Value  \
        >::value, \
        ForwardIterator \
    >::type

    template<typename ForwardIterator, typename Size, typename Value>
    inline
    GTFO_RESULT_OF_SEARCH_N(ForwardIterator,
                            Value)
    search_n(ForwardIterator _it_begin,
             ForwardIterator _it_end,
             Size            _count,
             const Value &   _value)
    {
        return ::std::search_n(::gtfo::move(_it_begin),
                               ::gtfo::move(_it_end),
                               ::gtfo::move(_count),
                               _value);
    }

    template<typename ForwardIterator, typename Size, typename Value, typename BinaryPredicate>
    inline
    ForwardIterator
    search_n(ForwardIterator _it_begin,
             ForwardIterator _it_end,
             Size            _count,
             const Value &   _value,
             BinaryPredicate _pred)
    {
        return ::std::search_n(::gtfo::move(_it_begin),
                               ::gtfo::move(_it_end),
                               ::gtfo::move(_count),
                               _value,
                               ::gtfo::move(_pred));
    }

    template<typename Range, typename Size, typename Value>
    inline
    typename _tt::iterator_of_range< Range >::type
    search_n(Range &&      _range,
             Size          _count,
             const Value & _value)
    {
        return ::std::search_n(begin(_range),
                               end(_range),
                               ::gtfo::move(_count),
                               _value);
    }

    template<typename Range, typename Size, typename Value, typename BinaryPredicate>
    inline
    GTFO_RESULT_OF_SEARCH_N_PRED(typename _tt::iterator_of_range< Range >::type,
                                 Value,
                                 BinaryPredicate)
    search_n(Range &&        _range,
             Size            _count,
             const Value &   _value,
             BinaryPredicate _pred)
    {
        return ::std::search_n(begin(_range),
                               end(_range),
                               ::gtfo::move(_count),
                               _value,
                               ::gtfo::move(_pred));
    }

#undef GTFO_RESULT_OF_SEARCH_N_PRED
#undef GTFO_RESULT_OF_SEARCH_N

}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_SEARCH_N_HPP
