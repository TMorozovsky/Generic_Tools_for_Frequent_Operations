#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_GREATER_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_GREATER_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct greater
        {
            constexpr greater           ()                noexcept { }
            constexpr greater           (greater &&)      noexcept { }
            constexpr greater           (const greater &) noexcept { }
            inline greater & operator = (greater &&)      noexcept { return *this; }
            inline greater & operator = (const greater &) noexcept { return *this; }
            inline   ~greater()                           noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) > ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) > ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) > ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_GREATER_HPP
