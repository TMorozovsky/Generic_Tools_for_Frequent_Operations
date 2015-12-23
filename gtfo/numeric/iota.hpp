#ifndef GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP
#define GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP

#include <numeric>
#include "gtfo/_impl/type_traits/is_assignable.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_container.hpp"
#include "gtfo/_impl/type_traits/value_of_dereferenced.hpp"
#include "gtfo/_impl/type_traits/value_from_container.hpp"

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
        ::std::iota(it_begin, it_end,
                    init);
    }

    template<typename ForwardIterator>
    inline
    GTFO_RESULT_OF_IOTA(ForwardIterator,
                        typename _tt::value_of_dereferenced< ForwardIterator >::type)
    iota(ForwardIterator it_begin,
         ForwardIterator it_end)
    {
        ::std::iota(it_begin, it_end,
                    typename _tt::value_of_dereferenced< ForwardIterator >::type());
    }

    template<typename Container, typename Value>
    inline
    GTFO_RESULT_OF_IOTA(typename _tt::iterator_of_container< Container >::type,
                        Value)
    iota(Container && container,
         Value        init)
    {
        ::std::iota(begin(container), end(container),
                    init);
    }

    template<typename Container>
    inline
    GTFO_RESULT_OF_IOTA(typename _tt::iterator_of_container< Container >::type,
                        typename _tt::value_from_container< Container >::type)
    iota(Container && container)
    {
        ::std::iota(begin(container), end(container),
                    typename _tt::value_from_container< Container >::type());
    }

#undef GTFO_RESULT_OF_IOTA

}

#endif // GTFO_FILE_INCLUDED_NUMERIC_IOTA_HPP

