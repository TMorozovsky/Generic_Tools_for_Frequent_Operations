#ifndef MATRIX_UTILS_MATRIX_ROWS_ITERATOR_HPP_INCLUDED
#define MATRIX_UTILS_MATRIX_ROWS_ITERATOR_HPP_INCLUDED

#include "gtfo/_impl/container/matrix/matrix_rows_iterator_conversion_mixin.hpp"
#include "gtfo/_impl/container/matrix/matrix_rows_reverse_iterator_base_mixin.hpp"
#include <iterator>

namespace gtfo
{
namespace detail
{
namespace container
{
    // Template that is used to generate different matrix rows iterator types.
    // Stores an InternalRowElementsIterator and the offset value of type DifferenceType.
    // ExtraConversionDestType is used by non-const iterators to enable conversions to their const counterparts;
    // ExtraBaseIteratorType is used by reverse iterators to implement both the base() member function
    // and the constructor which creates a reverse iterator from its base iterator.
    template<typename InternalRowElementsIterator, typename DifferenceType, typename ExtraConversionDestType = void, typename ExtraBaseIteratorType = void>
    class matrix_rows_iterator : public matrix_rows_iterator_conversion_mixin  <matrix_rows_iterator<InternalRowElementsIterator, DifferenceType, ExtraConversionDestType, ExtraBaseIteratorType>, ExtraConversionDestType>,
                                 public matrix_rows_reverse_iterator_base_mixin<matrix_rows_iterator<InternalRowElementsIterator, DifferenceType, ExtraConversionDestType, ExtraBaseIteratorType>, ExtraBaseIteratorType>
    {
        template<typename SelfType, typename DestType>         friend class matrix_rows_iterator_conversion_mixin;
        template<typename SelfType, typename BaseIteratorType> friend class matrix_rows_reverse_iterator_base_mixin;

        using _conversion_mixin     = matrix_rows_iterator_conversion_mixin  <matrix_rows_iterator<InternalRowElementsIterator, DifferenceType, ExtraConversionDestType, ExtraBaseIteratorType>, ExtraConversionDestType>;
        using _base_iterator_mixin  = matrix_rows_reverse_iterator_base_mixin<matrix_rows_iterator<InternalRowElementsIterator, DifferenceType, ExtraConversionDestType, ExtraBaseIteratorType>, ExtraBaseIteratorType>;

        using _base_iterator_type   = typename _base_iterator_mixin::_base_iterator_type;

    public:
        using iterator_category     = ::std::random_access_iterator_tag;
        using row_elements_iterator = InternalRowElementsIterator;
        using difference_type       = DifferenceType;
        using row_elements_range    = ::gtfo::iterator_range<row_elements_iterator>;

    private:
        row_elements_iterator       _current_row_begin;
        difference_type             _offset_between_rows;

        static_assert               (_tt::is_nothrow_default_constructible<difference_type>::value, "");
        static_assert               (_tt::is_nothrow_copy_constructible<difference_type>::value, "");
        static_assert               (_tt::is_nothrow_move_constructible<difference_type>::value, "");
        static_assert               (_tt::is_nothrow_constructible<difference_type, int>::value, "");
        static_assert               (_tt::is_nothrow_copy_assignable<difference_type>::value, "");
        static_assert               (_tt::is_nothrow_move_assignable<difference_type>::value, "");
        static_assert               (noexcept(_tt::declval<const difference_type &>() == _tt::declval<const difference_type &>()), "");
        static_assert               (noexcept(_tt::declval<const difference_type &>() != _tt::declval<const difference_type &>()), "");
        static_assert               (noexcept(_tt::declval<const difference_type &>() * _tt::declval<const difference_type &>()), "");
        static_assert               (_tt::is_same<_tt::decay_t<decltype(_tt::declval<const difference_type &>() * _tt::declval<const difference_type &>())>, difference_type>::value, "");

    public:
        constexpr matrix_rows_iterator(row_elements_iterator matrix_row_begin, difference_type offset_between_rows)
                                       noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                                                _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
                                   : _conversion_mixin    ()
                                   , _base_iterator_mixin ()
                                   , _current_row_begin   (::gtfo::move(matrix_row_begin))
                                   , _offset_between_rows (::gtfo::move(offset_between_rows)) { }

        constexpr matrix_rows_iterator(const matrix_rows_iterator & other)
                                       noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value)
                                   : _conversion_mixin    ()
                                   , _base_iterator_mixin ()
                                   , _current_row_begin   (other._current_row_begin)
                                   , _offset_between_rows (other._offset_between_rows) { }

        constexpr matrix_rows_iterator(matrix_rows_iterator && other)
                                       noexcept(_tt::is_nothrow_move_constructible<row_elements_iterator>::value)
                                   : _conversion_mixin    ()
                                   , _base_iterator_mixin ()
                                   , _current_row_begin   (::gtfo::move(other._current_row_begin))
                                   , _offset_between_rows (::gtfo::move(other._offset_between_rows)) { }

        template <typename BaseIteratorType, typename = typename _tt::enable_if<
                                                                     _tt::is_same<
                                                                         _tt::decay_t<BaseIteratorType>,
                                                                         _tt::decay_t<_base_iterator_type>
                                                                     >::value
                                                                 >::type >
        constexpr matrix_rows_iterator(BaseIteratorType && base_iterator)
                                       noexcept(noexcept(_base_iterator_mixin::_make_reverse_iterator(_tt::declval<BaseIteratorType>())) &&
                                                _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
                                   : matrix_rows_iterator (_base_iterator_mixin::_make_reverse_iterator(::gtfo::forward<BaseIteratorType>(base_iterator))) { }

    public:
        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator = (const matrix_rows_iterator & other)
            noexcept(_tt::is_nothrow_copy_assignable<row_elements_iterator>::value)
        {
            _current_row_begin   = other._current_row_begin;
            _offset_between_rows = other._offset_between_rows;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator = (matrix_rows_iterator && other)
            noexcept(_tt::is_nothrow_move_assignable<row_elements_iterator>::value)
        {
            _current_row_begin   = ::gtfo::move(other._current_row_begin);
            _offset_between_rows = ::gtfo::move(other._offset_between_rows);
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator ++ ()
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type &>()))
        {
            _current_row_begin += _offset_between_rows;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator operator ++ (int)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            matrix_rows_iterator tmp(*this);
            ++*this;
            return tmp;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator -- ()
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type &>()))
        {
            _current_row_begin -= _offset_between_rows;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator operator -- (int)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            matrix_rows_iterator tmp(*this);
            --*this;
            return tmp;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator += (difference_type diff)
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type>()))
        {
            _current_row_begin += diff * _offset_between_rows;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator -= (difference_type diff)
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type>()))
        {
            _current_row_begin -= diff * _offset_between_rows;
            return *this;
        }

        friend GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator operator + (matrix_rows_iterator orig, difference_type diff)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            orig += diff;
            return orig;
        }

        friend GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator operator + (difference_type diff, matrix_rows_iterator orig)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            orig += diff;
            return orig;
        }

        friend GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator operator - (matrix_rows_iterator orig, difference_type diff)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            orig -= diff;
            return orig;
        }

        friend GTFO_CONSTEXPR_FUNCTION difference_type operator - (const matrix_rows_iterator & lhs, const matrix_rows_iterator & rhs)
            noexcept(noexcept(lhs._current_row_begin - rhs._current_row_begin) &&
                     noexcept(_tt::declval<decltype(lhs._current_row_begin - rhs._current_row_begin)>() / lhs._offset_between_rows))
        {
            GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
            GTFO_DEBUG_ASSERT((lhs._offset_between_rows == difference_type(0)) || ((lhs._current_row_begin - rhs._current_row_begin) % lhs._offset_between_rows == 0));
            return (lhs._offset_between_rows == difference_type(0)) ? difference_type(0) : ((lhs._current_row_begin - rhs._current_row_begin) / lhs._offset_between_rows);
        }

        GTFO_CONSTEXPR_FUNCTION row_elements_range operator * () const
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() + _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_constructible<row_elements_range, row_elements_iterator &, row_elements_iterator>::value &&
                     _tt::is_nothrow_move_constructible<row_elements_range>::value)
        {
            static_assert(_tt::is_same<_tt::decay_t<decltype(_current_row_begin + _offset_between_rows)>, row_elements_iterator>::value, "");
            return row_elements_range(_current_row_begin, _current_row_begin + _offset_between_rows);
        }

#define GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(op) \
        friend GTFO_CONSTEXPR_FUNCTION bool operator op (const matrix_rows_iterator & lhs, const matrix_rows_iterator & rhs) \
            noexcept(noexcept(lhs._current_row_begin op rhs._current_row_begin)) \
        { \
            assert(lhs._offset_between_rows == rhs._offset_between_rows); \
            return lhs._current_row_begin op rhs._current_row_begin; \
        }

        GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(==)
        GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(!=)
        GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(<)
        GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(<=)
        GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(>)
        GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR(>=)

#undef GTFO_MATRIX_ROWS_ITERATOR_DEFINE_OVERLOADED_COMPARISON_OPERATOR
    };
}
}
}

#endif // MATRIX_UTILS_MATRIX_ROWS_ITERATOR_HPP_INCLUDED
