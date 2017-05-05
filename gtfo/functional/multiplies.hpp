#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_MULTIPLIES_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_MULTIPLIES_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct multiplies
        {
            constexpr multiplies           ()                   noexcept { }
            constexpr multiplies           (multiplies &&)      noexcept { }
            constexpr multiplies           (const multiplies &) noexcept { }
            inline multiplies & operator = (multiplies &&)      noexcept { return *this; }
            inline multiplies & operator = (const multiplies &) noexcept { return *this; }
            inline   ~multiplies()                              noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) * ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) * ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) * ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_MULTIPLIES_HPP
