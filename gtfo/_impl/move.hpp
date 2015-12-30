#ifndef GTFO_FILE_INCLUDED_MOVE_HPP
#define GTFO_FILE_INCLUDED_MOVE_HPP

#include "gtfo/_impl/type_traits/_common_definitions.hpp"

namespace gtfo
{
    template<typename T>
    GTFO_CONSTEXPR_FUNCTION
    typename _tt::remove_reference<T>::type &&
    move(T && arg) GTFO_NOEXCEPT
    {
        return static_cast< typename _tt::remove_reference<T>::type && >(arg);
    }
}

#endif // GTFO_FILE_INCLUDED_MOVE_HPP

