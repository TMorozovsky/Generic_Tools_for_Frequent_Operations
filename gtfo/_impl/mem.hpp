#ifndef GTFO_FILE_INCLUDED_MEM_HPP
#define GTFO_FILE_INCLUDED_MEM_HPP

#include "gtfo/_impl/_common_definitions.hpp"
#include <cstring>

namespace gtfo
{
    using ::std::memset;

    inline void memcpy_s(void * GTFO_RESTRICT dest, size_t dest_size, const void * GTFO_RESTRICT src, size_t src_size) noexcept
    {
        GTFO_DEBUG_ASSERT(dest_size >= src_size);
#ifdef _MSC_VER
        errno_t err = ::memcpy_s(dest, dest_size, src, src_size);
        GTFO_DEBUG_ASSERT(!err);
        (void)err;
#else
        (void)dest_size;
        ::std::memcpy(dest, src, src_size);
#endif
    }
}

#endif // GTFO_FILE_INCLUDED_MEM_HPP
