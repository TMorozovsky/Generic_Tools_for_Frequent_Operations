#ifndef GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP

/*
 * Defines the following overloads:
 *     iota(ForwardIterator, ForwardIterator);
 *     iota(ForwardIterator, ForwardIterator, Value);
 *     iota(Range);
 *     iota(Range,           Value);
 */

#include <numeric>
#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"
#include "gtfo/_impl/type_traits/value_from_range.hpp"

namespace gtfo
{

#define GTFO_RESULT_OF_IOTA(ForwardIterator, Value) \
    typename _tt::enable_if                                \
    <                                                            \
        _tt::is_incrementable< Value >::value                          \
        &&                                                                 \
        _tt::is_assignable                                                   \
        <                                                                      \
            typename _tt::result_of_dereferencing< ForwardIterator > ::type,    \
            Value                                                              \
        >::value,                                                         \
        void                                                        \
    >::type

    template<typename ForwardIterator>
    inline
    GTFO_RESULT_OF_IOTA(ForwardIterator,
                        typename _tt::value_of_dereferenced< ForwardIterator >::type)
    iota(ForwardIterator it_begin,
         ForwardIterator it_end)
    {
        ::std::iota(_utils::move(it_begin),
                    _utils::move(it_end),
                    typename _tt::value_of_dereferenced< ForwardIterator >::type());
    }

    template<typename ForwardIterator, typename Value>
    inline
    void
    iota(ForwardIterator it_begin,
         ForwardIterator it_end,
         Value           init)
    {
        ::std::iota(_utils::move(it_begin),
                    _utils::move(it_end),
                    _utils::move(init));
    }

    template<typename Range>
    inline
    void
    iota(Range && range)
    {
        ::std::iota(begin(range),
                    end(range),
                    typename _tt::value_from_range< Range >::type());
    }

    template<typename Range, typename Value>
    inline
    GTFO_RESULT_OF_IOTA(typename _tt::iterator_of_range< Range >::type,
                        Value)
    iota(Range && range,
         Value    init)
    {
        ::std::iota(begin(range),
                    end(range),
                    _utils::move(init));
    }

#undef GTFO_RESULT_OF_IOTA

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP
