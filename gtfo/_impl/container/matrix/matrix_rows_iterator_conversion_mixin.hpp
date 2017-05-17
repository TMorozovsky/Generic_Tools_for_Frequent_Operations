#ifndef GTFO_FILE_INCLUDED_CONTAINER_MATRIX_ROWS_ITERATOR_CONVERSION_MIXIN_HPP
#define GTFO_FILE_INCLUDED_CONTAINER_MATRIX_ROWS_ITERATOR_CONVERSION_MIXIN_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
namespace detail
{
namespace container
{
    // Allows the rows iterator which inherits from this mixin to be converted to another similar rows iterator type.
    // Typically used by non-const iterators for conversions to const_iterators.
    template<typename SelfType, typename DestType>
    class matrix_rows_iterator_conversion_mixin
    {
        static_assert(!_tt::is_reference<SelfType>::value, "");
        static_assert(!_tt::is_const    <SelfType>::value, "");
        static_assert(!_tt::is_volatile <SelfType>::value, "");
        static_assert(!_tt::is_reference<DestType>::value, "");
        static_assert(!_tt::is_const    <DestType>::value, "");
        static_assert(!_tt::is_volatile <DestType>::value, "");
        static_assert(!_tt::is_same<SelfType, DestType>::value, "");

    public:
        constexpr operator DestType() const
            noexcept(_tt::is_nothrow_constructible<DestType,
                                                   decltype(_tt::declval<const SelfType &>()._current_row_begin),
                                                   decltype(_tt::declval<const SelfType &>()._offset_between_rows)>::value &&
                     _tt::is_nothrow_move_constructible<DestType>::value)
        {
            return DestType( static_cast<const SelfType &>(*this)._current_row_begin,
                             static_cast<const SelfType &>(*this)._offset_between_rows );
        }

    protected:
        constexpr               matrix_rows_iterator_conversion_mixin()                                                            noexcept { }
        constexpr               matrix_rows_iterator_conversion_mixin(const matrix_rows_iterator_conversion_mixin &)               noexcept { }
        constexpr               matrix_rows_iterator_conversion_mixin(matrix_rows_iterator_conversion_mixin &&)                    noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator_conversion_mixin & operator = (const matrix_rows_iterator_conversion_mixin &) noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator_conversion_mixin & operator = (matrix_rows_iterator_conversion_mixin &&)      noexcept { }
    };

    template<typename SelfType>
    class matrix_rows_iterator_conversion_mixin<SelfType, void>
    {
    protected:
        constexpr               matrix_rows_iterator_conversion_mixin()                                                            noexcept { }
        constexpr               matrix_rows_iterator_conversion_mixin(const matrix_rows_iterator_conversion_mixin &)               noexcept { }
        constexpr               matrix_rows_iterator_conversion_mixin(matrix_rows_iterator_conversion_mixin &&)                    noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator_conversion_mixin & operator = (const matrix_rows_iterator_conversion_mixin &) noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator_conversion_mixin & operator = (matrix_rows_iterator_conversion_mixin &&)      noexcept { }
    };
}
}
}

#endif // GTFO_FILE_INCLUDED_CONTAINER_MATRIX_ROWS_ITERATOR_CONVERSION_MIXIN_HPP
