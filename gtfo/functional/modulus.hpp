#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_MODULUS_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_MODULUS_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct modulus
        {
            constexpr modulus           ()                noexcept { }
            constexpr modulus           (modulus &&)      noexcept { }
            constexpr modulus           (const modulus &) noexcept { }
            inline modulus & operator = (modulus &&)      noexcept { return *this; }
            inline modulus & operator = (const modulus &) noexcept { return *this; }
            inline   ~modulus()                           noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) % ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) % ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) % ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_MODULUS_HPP
