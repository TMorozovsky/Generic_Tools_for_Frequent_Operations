#ifndef GTFO_FILE_INCLUDED_ALGORITHM_HPP
#define GTFO_FILE_INCLUDED_ALGORITHM_HPP

#include <algorithm>
#include "type_traits.hpp"

namespace gtfo
{
    using ::std::for_each;

    template<typename Container, typename UnaryFunction>
    inline
    typename tt::enable_if
    <
        tt::is_container<Container>::value,
        UnaryFunction
    >::type
    for_each(const Container & container, UnaryFunction fun)
    {
        return ::std::for_each(begin(container), end(container), fun);
    }
}

#endif // GTFO_FILE_INCLUDED_ALGORITHM_HPP

