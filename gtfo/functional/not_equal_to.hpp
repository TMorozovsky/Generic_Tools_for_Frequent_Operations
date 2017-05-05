#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_NOT_EQUAL_TO_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_NOT_EQUAL_TO_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    inline namespace functional
    {
        struct not_equal_to
        {
            constexpr not_equal_to           ()                     noexcept { }
            constexpr not_equal_to           (not_equal_to &&)      noexcept { }
            constexpr not_equal_to           (const not_equal_to &) noexcept { }
            inline not_equal_to & operator = (not_equal_to &&)      noexcept { return *this; }
            inline not_equal_to & operator = (const not_equal_to &) noexcept { return *this; }
            inline   ~not_equal_to()                                noexcept { }

            typedef int is_transparent;

            template<typename Lhs, typename Rhs>
            constexpr auto operator () (Lhs && lhs, Rhs && rhs) const
                noexcept(noexcept(::gtfo::forward<Lhs>(lhs) != ::gtfo::forward<Rhs>(rhs)))
                      -> decltype(::gtfo::forward<Lhs>(lhs) != ::gtfo::forward<Rhs>(rhs))
            {
                return            ::gtfo::forward<Lhs>(lhs) != ::gtfo::forward<Rhs>(rhs);
            }
        };
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_NOT_EQUAL_TO_HPP
