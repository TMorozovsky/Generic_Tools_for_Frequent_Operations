#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_MINUS_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_MINUS_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct minus
        {
            constexpr minus           ()              noexcept { }
            constexpr minus           (minus &&)      noexcept { }
            constexpr minus           (const minus &) noexcept { }
            inline minus & operator = (minus &&)      noexcept { return *this; }
            inline minus & operator = (const minus &) noexcept { return *this; }
            inline   ~minus()                         noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) - ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) - ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) - ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_MINUS_HPP
