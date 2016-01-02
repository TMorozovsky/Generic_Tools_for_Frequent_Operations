#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP

#include "gtfo/_impl/_common_definitions.hpp"
#include <type_traits>

namespace gtfo
{
    namespace _tt
    {
        using ::std::enable_if;
        using ::std::is_same;
        using ::std::is_void;
        using ::std::is_const;
        using ::std::is_fundamental;
        using ::std::remove_reference;
        using ::std::is_lvalue_reference;
        using ::std::decay;
        using ::std::declval;

        typedef char(&yes_type)[1];
        typedef char(&no_type) [2];
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_DEFINITIONS_HPP
