#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_LESS_EQUAL_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_LESS_EQUAL_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct less_equal
        {
            constexpr less_equal           ()                   noexcept { }
            constexpr less_equal           (less_equal &&)      noexcept { }
            constexpr less_equal           (const less_equal &) noexcept { }
            inline less_equal & operator = (less_equal &&)      noexcept { return *this; }
            inline less_equal & operator = (const less_equal &) noexcept { return *this; }
            inline   ~less_equal()                              noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) <= ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) <= ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) <= ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_LESS_EQUAL_HPP
