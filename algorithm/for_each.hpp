#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP

#include <algorithm>
#include "type_traits.hpp"

namespace gtfo
{
#define GTFO_DISABLE_ADL(t) typename tt::type_of<t>::type

    template<typename InputIterator, typename UnaryFunction>
    inline
    typename tt::enable_if
    <
        tt::is_invokable_1
        <
            UnaryFunction,
            typename tt::result_of_dereferencing<InputIterator>::type
        >::value,
        UnaryFunction
    >::type
    for_each(GTFO_DISABLE_ADL(InputIterator) it_begin,
             GTFO_DISABLE_ADL(InputIterator) it_end,
             GTFO_DISABLE_ADL(UnaryFunction) fun)
    {
        return ::std::for_each(it_begin, it_end, fun);
    }

    template<typename Container, typename UnaryFunction>
    inline
    typename tt::enable_if
    <
        tt::is_invokable_1
        <
            UnaryFunction,
            typename tt::result_of_dereferencing
            <
                typename tt::iterator_of_container<Container>::type
            >::type
        >::value,
        UnaryFunction
    >::type
    for_each(Container && container, UnaryFunction fun)
    {
        return ::std::for_each(begin(container), end(container), fun);
    }

#undef GTFO_DISABLE_ADL
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
