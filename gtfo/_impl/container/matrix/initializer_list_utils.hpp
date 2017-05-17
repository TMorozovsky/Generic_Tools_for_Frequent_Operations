#ifndef GTFO_FILE_INCLUDED_CONTAINER_MATRIX_INITIALIZER_LIST_UTILS_HPP
#define GTFO_FILE_INCLUDED_CONTAINER_MATRIX_INITIALIZER_LIST_UTILS_HPP

#include "gtfo/_impl/_common_definitions.hpp"
#include <initializer_list>

namespace gtfo
{
namespace detail
{
    template<typename IL>
    GTFO_CONSTEXPR_FUNCTION auto get_2d_initializer_list_total_size(const IL & il) -> typename IL::size_type
    {
        using size_type = typename IL::size_type;
        size_type sz(0);
        for (const auto & sub_il : il)
            sz += sub_il.size();
        return sz;
    }

    template<typename IL>
    GTFO_CONSTEXPR_FUNCTION auto get_2d_initializer_list_num_cols(const IL & il) -> typename IL::size_type
    {
        using size_type = typename IL::size_type;
        size_type sz(0);
        if (il.size()) {
            auto iter = il.begin();
            const auto iter_end = il.end();
            sz = iter->size();
            ++iter;
            for ( ; iter != iter_end; ++iter) {
                if (iter->size() != sz) {
                    sz = size_type(-1);
                    break;
                }
            }
        }
        return sz;
    }
}
}

#endif // GTFO_FILE_INCLUDED_CONTAINER_MATRIX_INITIALIZER_LIST_UTILS_HPP
