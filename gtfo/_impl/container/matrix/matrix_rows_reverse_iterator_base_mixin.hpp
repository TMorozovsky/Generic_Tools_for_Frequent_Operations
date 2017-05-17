#ifndef GTFO_FILE_INCLUDED_CONTAINER_MATRIX_ROWS_REVERSE_ITERATOR_BASE_MIXIN_HPP
#define GTFO_FILE_INCLUDED_CONTAINER_MATRIX_ROWS_REVERSE_ITERATOR_BASE_MIXIN_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"

namespace gtfo
{
namespace detail
{
namespace container
{
    // Allows reverse_iterators of matrix rows to be converted to their respective base iterators
    // by providing the base() member function to the class of reverse_iterator.
    template<typename SelfType, typename BaseIteratorType>
    class matrix_rows_reverse_iterator_base_mixin
    {
        static_assert(!_tt::is_reference<SelfType>::value, "");
        static_assert(!_tt::is_const    <SelfType>::value, "");
        static_assert(!_tt::is_volatile <SelfType>::value, "");
        static_assert(!_tt::is_reference<BaseIteratorType>::value, "");
        static_assert(!_tt::is_const    <BaseIteratorType>::value, "");
        static_assert(!_tt::is_volatile <BaseIteratorType>::value, "");
        static_assert(!_tt::is_same<SelfType, BaseIteratorType>::value, "");

    public:
        constexpr BaseIteratorType base() const
            noexcept(_tt::is_nothrow_constructible<BaseIteratorType,
                                                   decltype(_tt::declval<const SelfType &>()._current_row_begin.base()),
                                                   decltype(_tt::declval<const SelfType &>()._offset_between_rows)>::value &&
                     _tt::is_nothrow_move_constructible<BaseIteratorType>::value)
        {
            return BaseIteratorType( static_cast<const SelfType &>(*this)._current_row_begin.base(),
                                     static_cast<const SelfType &>(*this)._offset_between_rows );
        }

    protected:
        constexpr static SelfType _make_reverse_iterator(BaseIteratorType source)
            noexcept(_tt::is_nothrow_constructible<typename SelfType::row_elements_iterator,
                                                   decltype(::gtfo::move(source._current_row_begin)),
                                                   decltype(::gtfo::move(source._offset_between_rows))>::value &&
                     _tt::is_nothrow_constructible<SelfType, typename SelfType::row_elements_iterator>::value &&
                     _tt::is_nothrow_move_constructible<SelfType>::value)
        {
            return SelfType(typename SelfType::row_elements_iterator(::gtfo::move(source._current_row_begin)),
                                                                     ::gtfo::move(source._offset_between_rows));
        }

        using _base_iterator_type = BaseIteratorType;

    protected:
        constexpr               matrix_rows_reverse_iterator_base_mixin()                                                              noexcept { }
        constexpr               matrix_rows_reverse_iterator_base_mixin(const matrix_rows_reverse_iterator_base_mixin &)               noexcept { }
        constexpr               matrix_rows_reverse_iterator_base_mixin(matrix_rows_reverse_iterator_base_mixin &&)                    noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_reverse_iterator_base_mixin & operator = (const matrix_rows_reverse_iterator_base_mixin &) noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_reverse_iterator_base_mixin & operator = (matrix_rows_reverse_iterator_base_mixin &&)      noexcept { }
    };

    template<typename SelfType>
    class matrix_rows_reverse_iterator_base_mixin<SelfType, void>
    {
    protected:
        using _base_iterator_type = void;

    protected:
        constexpr               matrix_rows_reverse_iterator_base_mixin()                                                              noexcept { }
        constexpr               matrix_rows_reverse_iterator_base_mixin(const matrix_rows_reverse_iterator_base_mixin &)               noexcept { }
        constexpr               matrix_rows_reverse_iterator_base_mixin(matrix_rows_reverse_iterator_base_mixin &&)                    noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_reverse_iterator_base_mixin & operator = (const matrix_rows_reverse_iterator_base_mixin &) noexcept { }
        GTFO_CONSTEXPR_FUNCTION matrix_rows_reverse_iterator_base_mixin & operator = (matrix_rows_reverse_iterator_base_mixin &&)      noexcept { }
    };
}
}
}

#endif // GTFO_FILE_INCLUDED_CONTAINER_MATRIX_ROWS_REVERSE_ITERATOR_BASE_MIXIN_HPP
