#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_DIVIDES_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_DIVIDES_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct divides
        {
            constexpr divides           ()                noexcept { }
            constexpr divides           (divides &&)      noexcept { }
            constexpr divides           (const divides &) noexcept { }
            inline divides & operator = (divides &&)      noexcept { return *this; }
            inline divides & operator = (const divides &) noexcept { return *this; }
            inline   ~divides()                           noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) / ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) / ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) / ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_DIVIDES_HPP
