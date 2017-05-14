#ifndef GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP
#define GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <cassert>

namespace gtfo
{
    using ::std::size_t;
    using ::std::ptrdiff_t;
    using ::std::uintptr_t;

    using ::std::begin;
    using ::std::end;
}

#if defined(_MSC_VER) && !defined(__clang__) && (_MSC_VER <= 1900)
#   define GTFO_CONSTEXPR_FUNCTION inline
#else
#   define GTFO_CONSTEXPR_FUNCTION constexpr
#endif

#define GTFO_DEBUG_ASSERT(x) \
    { \
        assert((x)); \
    }

#endif // GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP
