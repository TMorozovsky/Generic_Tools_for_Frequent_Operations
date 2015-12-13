#ifndef GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP

#include <algorithm>
#include "gtfo/_impl/type_traits/is_invokable_fun1.hpp"
#include "gtfo/_impl/type_traits/result_of_dereferencing.hpp"
#include "gtfo/_impl/type_traits/iterator_of_container.hpp"

namespace gtfo
{
    template<typename Container, typename UnaryFunction>
    inline
    typename _tt::enable_if
    <
        _tt::is_invokable_fun1
        <
            UnaryFunction,
            typename _tt::result_of_dereferencing
            <
                typename _tt::iterator_of_container<Container>::type
            >::type
        >::value,
        UnaryFunction
    >::type
    for_each(Container && container, UnaryFunction fun)
    {
        return ::std::for_each(begin(container), end(container), fun);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_FOR_EACH_HPP
