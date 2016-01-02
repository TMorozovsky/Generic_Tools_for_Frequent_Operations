#ifndef GTFO_FILE_INCLUDED_ADDRESSOF_HPP
#define GTFO_FILE_INCLUDED_ADDRESSOF_HPP

#include "gtfo/_impl/_common_definitions.hpp"

namespace gtfo
{
    template<typename T>
    inline
    T *
    addressof(T & arg) GTFO_NOEXCEPT
    {
        return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
    }
}

#endif // GTFO_FILE_INCLUDED_ADDRESSOF_HPP

