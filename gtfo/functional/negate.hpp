#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_NEGATE_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_NEGATE_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct negate
        {
            constexpr negate           ()               noexcept { }
            constexpr negate           (negate &&)      noexcept { }
            constexpr negate           (const negate &) noexcept { }
            inline negate & operator = (negate &&)      noexcept { return *this; }
            inline negate & operator = (const negate &) noexcept { return *this; }
            inline   ~negate()                          noexcept { }

            typedef int is_transparent;

            template<typename Arg>
            constexpr auto operator () (Arg && arg) const
                noexcept(noexcept(-::gtfo::forward<Arg>(arg)))
                      -> decltype(-::gtfo::forward<Arg>(arg))
            {
                return            -::gtfo::forward<Arg>(arg);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_NEGATE_HPP
