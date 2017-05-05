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

#define GTFO_DEBUG_ASSERT(x) \
    { \
        assert((x)); \
    }

#endif // GTFO_FILE_INCLUDED_COMMON_DEFINITIONS_HPP
