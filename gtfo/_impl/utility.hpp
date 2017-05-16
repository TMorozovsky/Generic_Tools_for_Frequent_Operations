#ifndef GTFO_FILE_INCLUDED_UTILITY_HPP
#define GTFO_FILE_INCLUDED_UTILITY_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
    template<typename T>
    inline
    T *
    addressof(T & arg) noexcept
    {
        return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
    }

    template<typename T>
    constexpr
    _tt::remove_reference_t<T> &&
    move(T && arg) noexcept
    {
        return static_cast<_tt::remove_reference_t<T> &&>(arg);
    }

    template<typename T>
    constexpr
    T &&
    forward(_tt::remove_reference_t<T> & arg) noexcept
    {
        return static_cast<T &&>(arg);
    }

    template<typename T>
    constexpr
    T &&
    forward(_tt::remove_reference_t<T> && arg) noexcept
    {
        return static_cast<T &&>(arg);
    }
}

#endif // GTFO_FILE_INCLUDED_UTILITY_HPP
