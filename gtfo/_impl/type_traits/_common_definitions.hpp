#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_COMMON_DEFINITIONS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_COMMON_DEFINITIONS_HPP

#include <type_traits>
#include <iterator>

#if (__cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define GTFO_CONSTEXPR constexpr
#else
#define GTFO_CONSTEXPR const
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#define GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
#endif

namespace gtfo
{
    using ::std::begin;
    using ::std::end;

    namespace _tt
    {
        using ::std::enable_if;
        using ::std::is_same;
        using ::std::is_void;
        using ::std::is_const;
        using ::std::is_fundamental;
        using ::std::remove_reference;
        using ::std::decay;
        using ::std::declval;

        typedef char(&yes_type)[1];
        typedef char(&no_type) [2];
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_COMMON_DEFINITIONS_HPP
