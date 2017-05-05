#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_PLUS_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_PLUS_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct plus
        {
            constexpr plus           ()             noexcept { }
            constexpr plus           (plus &&)      noexcept { }
            constexpr plus           (const plus &) noexcept { }
            inline plus & operator = (plus &&)      noexcept { return *this; }
            inline plus & operator = (const plus &) noexcept { return *this; }
            inline   ~plus()                        noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) + ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) + ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) + ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_PLUS_HPP
