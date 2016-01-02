#ifndef GTFO_FILE_INCLUDED_UTILITY_HPP
#define GTFO_FILE_INCLUDED_UTILITY_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    template<typename T>
    inline
    T *
    addressof(T & arg) GTFO_NOEXCEPT
    {
        return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
    }

    template<typename T>
    GTFO_CONSTEXPR_FUNCTION
    typename _tt::remove_reference<T>::type &&
    move(T && arg) GTFO_NOEXCEPT
    {
        return static_cast< typename _tt::remove_reference<T>::type && >(arg);
    }

    template<typename T>
    GTFO_CONSTEXPR_FUNCTION
    T &&
    forward(typename _tt::remove_reference<T>::type & arg) GTFO_NOEXCEPT
    {
        return static_cast<T &&>(arg);
    }

    template<typename T>
    GTFO_CONSTEXPR_FUNCTION
    T &&
    forward(typename _tt::remove_reference<T>::type && arg) GTFO_NOEXCEPT
    {
        return static_cast<T &&>(arg);
    }
}

#endif // GTFO_FILE_INCLUDED_UTILITY_HPP
