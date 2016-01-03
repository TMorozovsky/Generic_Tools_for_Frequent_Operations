#ifndef GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP
#define GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP

#include <cstddef>
#include <iterator>

#if (__cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define GTFO_CONSTEXPR constexpr
#define GTFO_CONSTEXPR_FUNCTION constexpr
#define GTFO_NOEXCEPT noexcept
#else
#define GTFO_CONSTEXPR const
#define GTFO_CONSTEXPR_FUNCTION inline
#define GTFO_NOEXCEPT throw()
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#define GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
#endif

namespace gtfo
{
    using ::std::size_t;
    using ::std::ptrdiff_t;

    using ::std::begin;
    using ::std::end;
}

#endif // GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP
