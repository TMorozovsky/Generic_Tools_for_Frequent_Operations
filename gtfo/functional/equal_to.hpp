#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_EQUAL_TO_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_EQUAL_TO_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct equal_to
        {
            constexpr equal_to           ()                 noexcept { }
            constexpr equal_to           (equal_to &&)      noexcept { }
            constexpr equal_to           (const equal_to &) noexcept { }
            inline equal_to & operator = (equal_to &&)      noexcept { return *this; }
            inline equal_to & operator = (const equal_to &) noexcept { return *this; }
            inline   ~equal_to()                            noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) == ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) == ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) == ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_EQUAL_TO_HPP
