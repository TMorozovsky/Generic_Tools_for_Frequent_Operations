#ifndef GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP

#include <numeric>
#include "gtfo/_impl/move.hpp"
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

    template<typename ForwardIterator, typename Value>
    inline
    GTFO_RESULT_OF_IOTA(ForwardIterator,
                        Value)
    iota(ForwardIterator it_begin,
         ForwardIterator it_end,
         Value           init)
    {
        ::std::iota(::gtfo::move(it_begin),
                    ::gtfo::move(it_end),
                    ::gtfo::move(init));
    }

    template<typename ForwardIterator>
    inline
    GTFO_RESULT_OF_IOTA(ForwardIterator,
                        typename _tt::value_of_dereferenced< ForwardIterator >::type)
    iota(ForwardIterator it_begin,
         ForwardIterator it_end)
    {
        ::std::iota(::gtfo::move(it_begin),
                    ::gtfo::move(it_end),
                    typename _tt::value_of_dereferenced< ForwardIterator >::type());
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
                    ::gtfo::move(init));
    }

    template<typename Range>
    inline
    GTFO_RESULT_OF_IOTA(typename _tt::iterator_of_range< Range >::type,
                        typename _tt::value_from_range< Range >::type)
    iota(Range && range)
    {
        ::std::iota(begin(range),
                    end(range),
                    typename _tt::value_from_range< Range >::type());
    }

#undef GTFO_RESULT_OF_IOTA

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP

