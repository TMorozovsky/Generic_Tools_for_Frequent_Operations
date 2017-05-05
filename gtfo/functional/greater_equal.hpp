#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_GREATER_EQUAL_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_GREATER_EQUAL_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct greater_equal
        {
            constexpr greater_equal           ()                      noexcept { }
            constexpr greater_equal           (greater_equal &&)      noexcept { }
            constexpr greater_equal           (const greater_equal &) noexcept { }
            inline greater_equal & operator = (greater_equal &&)      noexcept { return *this; }
            inline greater_equal & operator = (const greater_equal &) noexcept { return *this; }
            inline   ~greater_equal()                                 noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) >= ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) >= ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) >= ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_GREATER_EQUAL_HPP
