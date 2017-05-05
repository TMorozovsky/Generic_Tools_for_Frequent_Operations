#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_LESS_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_LESS_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct less
        {
            constexpr less           ()             noexcept { }
            constexpr less           (less &&)      noexcept { }
            constexpr less           (const less &) noexcept { }
            inline less & operator = (less &&)      noexcept { return *this; }
            inline less & operator = (const less &) noexcept { return *this; }
            inline   ~less()                        noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) < ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) < ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) < ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_LESS_HPP
