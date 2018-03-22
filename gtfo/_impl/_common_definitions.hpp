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

    using ::std::input_iterator_tag;
    using ::std::output_iterator_tag;
    using ::std::forward_iterator_tag;
    using ::std::bidirectional_iterator_tag;
    using ::std::random_access_iterator_tag;

    using ::std::iterator_traits;
}

#if (__cplusplus >= 201402L) || defined(__clang__) || (defined(__GNUG__) && (__GNUC__ >= 5)) || (defined(_MSC_VER) && (_MSC_VER >= 1910))
#   define GTFO_CONSTEXPR_FUNCTION constexpr
#else
#   define GTFO_CONSTEXPR_FUNCTION inline
#   define GTFO_LIMITED_CONSTEXPR_SUPPORT
#endif

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUG__)
#   define GTFO_RESTRICT __restrict
#else
#   define GTFO_RESTRICT
#endif

#define GTFO_DEBUG_ASSERT(x) \
    { \
        assert((x)); \
    }

#endif // GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP
