#ifndef GTFO_FILE_INCLUDED_UTILS_SCOPE_EXIT_HPP
#define GTFO_FILE_INCLUDED_UTILS_SCOPE_EXIT_HPP

#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    namespace detail
    {
        template<typename F>
        class scope_guard
        {
            F _func;

        public:
            template<typename T>
            explicit GTFO_CONSTEXPR_FUNCTION scope_guard(T && func)
                noexcept(_tt::is_nothrow_constructible<F, T &&>::value)
                : _func(::gtfo::forward<T>(func))
            {
            }

            scope_guard() = delete;
            scope_guard(const scope_guard &) = delete;
            scope_guard & operator = (const scope_guard &) = delete;

            scope_guard(scope_guard &&) = default;
            scope_guard & operator = (scope_guard &&) = default;

            ~scope_guard()
            {
                _func();
            }
        };

        struct scope_guard_creator
        {
        };

        template<typename F>
        GTFO_CONSTEXPR_FUNCTION scope_guard<::gtfo::_tt::decay_t<F>> operator << (scope_guard_creator, F && func)
            noexcept(_tt::is_nothrow_constructible<scope_guard<::gtfo::_tt::decay_t<F>>, F &&>::value &&
                     _tt::is_nothrow_move_constructible<scope_guard<::gtfo::_tt::decay_t<F>>>::value)
        {
            return scope_guard<::gtfo::_tt::decay_t<F>>(::gtfo::forward<F>(func));
        }
    }
}

#define GTFO_IMPL_CONCATENATE_IDENTIFIER(l, r) l##r
#define GTFO_CONCATENATE_IDENTIFIER(l, r)      GTFO_IMPL_CONCATENATE_IDENTIFIER(l,r)
#define GTFO_IDENTIFIER_WITH_LINE(s)           GTFO_CONCATENATE_IDENTIFIER(s, __LINE__)

#define GTFO_SCOPE_EXIT \
    auto GTFO_IDENTIFIER_WITH_LINE(GTFO_scope_guard_at_line_) = ::gtfo::detail::scope_guard_creator() << [&] () -> void

#endif // GTFO_FILE_INCLUDED_UTILS_SCOPE_EXIT_HPP
