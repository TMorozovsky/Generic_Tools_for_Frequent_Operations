#ifndef MATRIX_UTILS_MATRIX_ROWS_ITERATOR_HPP_INCLUDED
#define MATRIX_UTILS_MATRIX_ROWS_ITERATOR_HPP_INCLUDED

#include "gtfo/_impl/_common_definitions.hpp"
#include "gtfo/_impl/iterator_range.hpp"
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
namespace detail
{
namespace container
{
    struct matrix_rows_iterator_common_base
    {
    public:
        using iterator_category = random_access_iterator_tag;

    protected:
        constexpr matrix_rows_iterator_common_base() noexcept { }
    };

    template< typename UnderlyingRowElementsIterator, typename DifferenceType,
              typename DerivedSelfType >
    class matrix_rows_iterator_base : public matrix_rows_iterator_common_base
    {
    public:
        using row_elements_iterator = UnderlyingRowElementsIterator;
        using row_elements_range    = iterator_range<UnderlyingRowElementsIterator>;

        using difference_type       = DifferenceType;
        using value_type            = row_elements_range;
        using reference             = const row_elements_range &;
        using pointer               = void *;

    public:
        row_elements_iterator       _current_row_begin;
        difference_type             _offset_between_rows;

        static_assert               (!_tt::is_reference<row_elements_iterator>::value, "");
        static_assert               (!_tt::is_const<row_elements_iterator>::value, "");
        static_assert               (!_tt::is_volatile<row_elements_iterator>::value, "");
        static_assert               (!_tt::is_reference<difference_type>::value, "");
        static_assert               (!_tt::is_const<difference_type>::value, "");
        static_assert               (!_tt::is_volatile<difference_type>::value, "");

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

    protected:
        constexpr matrix_rows_iterator_base()
                                       noexcept(_tt::is_nothrow_default_constructible<row_elements_iterator>::value)
                                   : _current_row_begin   ()
                                   , _offset_between_rows () { }

        template<typename RowElementsIterator>
        constexpr matrix_rows_iterator_base(RowElementsIterator && matrix_row_begin, difference_type offset_between_rows)
                                       noexcept(_tt::is_nothrow_constructible<row_elements_iterator, RowElementsIterator>::value)
                                   : _current_row_begin   (::gtfo::forward<RowElementsIterator>(matrix_row_begin))
                                   , _offset_between_rows (::gtfo::move(offset_between_rows)) { }

        constexpr matrix_rows_iterator_base(const matrix_rows_iterator_base & other)
                                       noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value)
                                   : _current_row_begin   (other._current_row_begin)
                                   , _offset_between_rows (other._offset_between_rows) { }

        constexpr matrix_rows_iterator_base(matrix_rows_iterator_base && other)
                                       noexcept(_tt::is_nothrow_move_constructible<row_elements_iterator>::value)
                                   : _current_row_begin   (::gtfo::move(other._current_row_begin))
                                   , _offset_between_rows (::gtfo::move(other._offset_between_rows)) { }

    public:
        GTFO_CONSTEXPR_FUNCTION void operator = (const matrix_rows_iterator_base & other)
            noexcept(_tt::is_nothrow_copy_assignable<row_elements_iterator>::value)
        {
            _current_row_begin   = other._current_row_begin;
            _offset_between_rows = other._offset_between_rows;
        }

        GTFO_CONSTEXPR_FUNCTION void operator = (matrix_rows_iterator_base && other)
            noexcept(_tt::is_nothrow_move_assignable<row_elements_iterator>::value)
        {
            _current_row_begin   = ::gtfo::move(other._current_row_begin);
            _offset_between_rows = ::gtfo::move(other._offset_between_rows);
        }

    public:
        GTFO_CONSTEXPR_FUNCTION DerivedSelfType & operator ++ ()
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type &>()))
        {
            _current_row_begin += _offset_between_rows;
            return static_cast<DerivedSelfType &>(*this);
        }

        GTFO_CONSTEXPR_FUNCTION DerivedSelfType operator ++ (int)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            DerivedSelfType tmp(static_cast<const DerivedSelfType &>(*this));
            ++*this;
            return tmp;
        }

        GTFO_CONSTEXPR_FUNCTION DerivedSelfType & operator -- ()
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type &>()))
        {
            _current_row_begin -= _offset_between_rows;
            return static_cast<DerivedSelfType &>(*this);
        }

        GTFO_CONSTEXPR_FUNCTION DerivedSelfType operator -- (int)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            DerivedSelfType tmp(static_cast<const DerivedSelfType &>(*this));
            --*this;
            return tmp;
        }

        GTFO_CONSTEXPR_FUNCTION DerivedSelfType & operator += (difference_type diff)
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type>()))
        {
            _current_row_begin += diff * _offset_between_rows;
            return static_cast<DerivedSelfType &>(*this);
        }

        GTFO_CONSTEXPR_FUNCTION DerivedSelfType & operator -= (difference_type diff)
            noexcept(noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type>()))
        {
            _current_row_begin -= diff * _offset_between_rows;
            return static_cast<DerivedSelfType &>(*this);
        }

        friend GTFO_CONSTEXPR_FUNCTION DerivedSelfType operator + (DerivedSelfType orig, difference_type diff)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            return orig += diff;
        }

        friend GTFO_CONSTEXPR_FUNCTION DerivedSelfType operator + (difference_type diff, DerivedSelfType orig)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() += _tt::declval<difference_type>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            return orig += diff;
        }

        friend GTFO_CONSTEXPR_FUNCTION DerivedSelfType operator - (DerivedSelfType orig, difference_type diff)
            noexcept(_tt::is_nothrow_copy_constructible<row_elements_iterator>::value &&
                     noexcept(_tt::declval<row_elements_iterator &>() -= _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_move_constructible<row_elements_iterator>::value)
        {
            return orig -= diff;
        }

        friend GTFO_CONSTEXPR_FUNCTION difference_type operator - (const matrix_rows_iterator_base & lhs, const matrix_rows_iterator_base & rhs)
            noexcept(noexcept(_tt::declval<const matrix_rows_iterator_base &>()._current_row_begin - _tt::declval<const matrix_rows_iterator_base &>()._current_row_begin) &&
                     noexcept(_tt::declval<decltype(_tt::declval<const matrix_rows_iterator_base &>()._current_row_begin - _tt::declval<const matrix_rows_iterator_base &>()._current_row_begin)>()
                                  / _tt::declval<const matrix_rows_iterator_base &>()._offset_between_rows))
        {
            GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
            GTFO_DEBUG_ASSERT((lhs._offset_between_rows == difference_type(0)) || ((lhs._current_row_begin - rhs._current_row_begin) % lhs._offset_between_rows == 0));
            GTFO_DEBUG_ASSERT(lhs._offset_between_rows != difference_type(0));
            return (lhs._current_row_begin - rhs._current_row_begin) / lhs._offset_between_rows;
        }

        GTFO_CONSTEXPR_FUNCTION row_elements_range operator * () const
            noexcept(noexcept(_tt::declval<const row_elements_iterator &>() + _tt::declval<const difference_type &>()) &&
                     _tt::is_nothrow_constructible<row_elements_range, row_elements_iterator &, row_elements_iterator>::value &&
                     _tt::is_nothrow_move_constructible<row_elements_range>::value)
        {
            static_assert(_tt::is_same<_tt::decay_t<decltype(_current_row_begin + _offset_between_rows)>, row_elements_iterator>::value, "");
            return row_elements_range(_current_row_begin, _current_row_begin + _offset_between_rows);
        }

    private:
        // result of 'diff * _offset_between_rows'
        using _helper_type_0 = decltype(_tt::declval<difference_type &>() * _tt::declval<const difference_type &>());

        // result of '_current_row_begin + diff * _offset_between_rows'
        using _helper_type_1 = decltype(_tt::declval<const row_elements_iterator &>() + _tt::declval<_helper_type_0>());

        // result of 'result_row_begin + _offset_between_rows'
        using _helper_type_2 = decltype(_tt::declval<const _helper_type_1>() + _tt::declval<const difference_type &>());

    public:
        GTFO_CONSTEXPR_FUNCTION row_elements_range operator [] (difference_type diff) const
            noexcept(noexcept(_tt::declval<const row_elements_iterator &>() + _tt::declval<_helper_type_0>()) &&
                     _tt::is_nothrow_constructible<row_elements_range, const _helper_type_1 &, _helper_type_2>::value &&
                     _tt::is_nothrow_move_constructible<row_elements_range>::value)
        {
            return row_elements_range(_current_row_begin + diff * _offset_between_rows,
                                      _current_row_begin + diff * _offset_between_rows + _offset_between_rows);
        }
    };

    template<typename UnderlyingRowElementsIterator, typename DifferenceType>
    class matrix_rows_iterator
        : public matrix_rows_iterator_base<
                     UnderlyingRowElementsIterator, DifferenceType,
                     matrix_rows_iterator<UnderlyingRowElementsIterator, DifferenceType>
                 >
    {
    private:
        using _base_type = matrix_rows_iterator_base<
                               UnderlyingRowElementsIterator, DifferenceType,
                               matrix_rows_iterator<UnderlyingRowElementsIterator, DifferenceType>
                           >;
    public:
        constexpr matrix_rows_iterator()
            noexcept(_tt::is_nothrow_default_constructible<_base_type>::value)
            : _base_type() { }

        template<typename RowElementsIterator>
        constexpr matrix_rows_iterator(RowElementsIterator && matrix_row_begin, DifferenceType offset_between_rows)
            noexcept(_tt::is_nothrow_constructible<_base_type, RowElementsIterator, DifferenceType>::value)
            : _base_type(::gtfo::forward<RowElementsIterator>(matrix_row_begin), ::gtfo::move(offset_between_rows)) { }

        constexpr matrix_rows_iterator(const matrix_rows_iterator & other)
            noexcept(_tt::is_nothrow_copy_constructible<_base_type>::value)
            : _base_type(static_cast<const _base_type &>(other)) { }

        constexpr matrix_rows_iterator(matrix_rows_iterator && other)
            noexcept(_tt::is_nothrow_move_constructible<_base_type>::value)
            : _base_type(::gtfo::move(static_cast<_base_type &&>(other))) { }

    public:
        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator = (const matrix_rows_iterator & other)
            noexcept(_tt::is_nothrow_copy_assignable<_base_type>::value)
        {
            _base_type::operator=(static_cast<const _base_type &>(other));
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_iterator & operator = (matrix_rows_iterator && other)
            noexcept(_tt::is_nothrow_move_assignable<_base_type>::value)
        {
            _base_type::operator=(::gtfo::move(static_cast<_base_type &&>(other)));
            return *this;
        }
    };

    template<typename UnderlyingRowElementsIterator, typename DifferenceType, typename NonConstIteratorType>
    class matrix_rows_const_iterator
        : public matrix_rows_iterator_base<
                     UnderlyingRowElementsIterator, DifferenceType,
                     matrix_rows_const_iterator<UnderlyingRowElementsIterator, DifferenceType, NonConstIteratorType>
                 >
    {
        static_assert(!_tt::is_reference<NonConstIteratorType>::value, "");
        static_assert(!_tt::is_const<NonConstIteratorType>::value, "");
        static_assert(!_tt::is_volatile<NonConstIteratorType>::value, "");
        static_assert(_tt::is_base_of<matrix_rows_iterator_common_base, NonConstIteratorType>::value, "");

    private:
        using _base_type = matrix_rows_iterator_base<
                               UnderlyingRowElementsIterator, DifferenceType,
                               matrix_rows_const_iterator<UnderlyingRowElementsIterator, DifferenceType, NonConstIteratorType>
                           >;
    public:
        constexpr matrix_rows_const_iterator()
            noexcept(_tt::is_nothrow_default_constructible<_base_type>::value)
            : _base_type() { }

        template<typename RowElementsIterator>
        constexpr matrix_rows_const_iterator(RowElementsIterator && matrix_row_begin, DifferenceType offset_between_rows)
            noexcept(_tt::is_nothrow_constructible<_base_type, RowElementsIterator, DifferenceType>::value)
            : _base_type(::gtfo::forward<RowElementsIterator>(matrix_row_begin),
                         ::gtfo::move(offset_between_rows)) { }

        constexpr matrix_rows_const_iterator(const matrix_rows_const_iterator & other)
            noexcept(_tt::is_nothrow_copy_constructible<_base_type>::value)
            : _base_type(static_cast<const _base_type &>(other)) { }

        constexpr matrix_rows_const_iterator(matrix_rows_const_iterator && other)
            noexcept(_tt::is_nothrow_move_constructible<_base_type>::value)
            : _base_type(::gtfo::move(static_cast<_base_type &&>(other))) { }

    public:
        constexpr matrix_rows_const_iterator(const NonConstIteratorType & non_const_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   const decltype(_tt::declval<const NonConstIteratorType &>()._current_row_begin) &,
                                                   const decltype(_tt::declval<const NonConstIteratorType &>()._offset_between_rows) &>::value)
            : _base_type(non_const_iterator._current_row_begin,
                         non_const_iterator._offset_between_rows) { }

        constexpr matrix_rows_const_iterator(NonConstIteratorType && non_const_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   decltype(::gtfo::move(_tt::declval<NonConstIteratorType &>()._current_row_begin)),
                                                   decltype(::gtfo::move(_tt::declval<NonConstIteratorType &>()._offset_between_rows))>::value)
            : _base_type(::gtfo::move(non_const_iterator._current_row_begin),
                         ::gtfo::move(non_const_iterator._offset_between_rows)) { }

    public:
        GTFO_CONSTEXPR_FUNCTION matrix_rows_const_iterator & operator = (const matrix_rows_const_iterator & other)
            noexcept(_tt::is_nothrow_copy_assignable<_base_type>::value)
        {
            _base_type::operator=(static_cast<const _base_type &>(other));
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_const_iterator & operator = (matrix_rows_const_iterator && other)
            noexcept(_tt::is_nothrow_move_assignable<_base_type>::value)
        {
            _base_type::operator=(::gtfo::move(static_cast<_base_type &&>(other)));
            return *this;
        }
    };

    template<typename UnderlyingRowElementsIterator, typename DifferenceType, typename BaseIteratorType>
    class matrix_rows_reverse_iterator
        : public matrix_rows_iterator_base<
                     UnderlyingRowElementsIterator, DifferenceType,
                     matrix_rows_reverse_iterator<UnderlyingRowElementsIterator, DifferenceType, BaseIteratorType>
                 >
    {
        static_assert(!_tt::is_reference<BaseIteratorType>::value, "");
        static_assert(!_tt::is_const<BaseIteratorType>::value, "");
        static_assert(!_tt::is_volatile<BaseIteratorType>::value, "");
        static_assert(_tt::is_base_of<matrix_rows_iterator_common_base, BaseIteratorType>::value, "");

    private:
        using _base_type = matrix_rows_iterator_base<
                               UnderlyingRowElementsIterator, DifferenceType,
                               matrix_rows_reverse_iterator<UnderlyingRowElementsIterator, DifferenceType, BaseIteratorType>
                           >;

    public:
        constexpr matrix_rows_reverse_iterator()
            noexcept(_tt::is_nothrow_default_constructible<_base_type>::value)
            : _base_type() { }

        template<typename RowElementsIterator>
        constexpr matrix_rows_reverse_iterator(RowElementsIterator && matrix_row_begin, DifferenceType offset_between_rows)
            noexcept(_tt::is_nothrow_constructible<_base_type, RowElementsIterator, DifferenceType>::value)
            : _base_type(::gtfo::forward<RowElementsIterator>(matrix_row_begin),
                         ::gtfo::move(offset_between_rows)) { }

        constexpr matrix_rows_reverse_iterator(const matrix_rows_reverse_iterator & other)
            noexcept(_tt::is_nothrow_copy_constructible<_base_type>::value)
            : _base_type(static_cast<const _base_type &>(other)) { }

        constexpr matrix_rows_reverse_iterator(matrix_rows_reverse_iterator && other)
            noexcept(_tt::is_nothrow_move_constructible<_base_type>::value)
            : _base_type(::gtfo::move(static_cast<_base_type &&>(other))) { }

    public:
        constexpr matrix_rows_reverse_iterator(const BaseIteratorType & base_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   const decltype(_tt::declval<const BaseIteratorType &>()._current_row_begin) &,
                                                   const decltype(_tt::declval<const BaseIteratorType &>()._offset_between_rows) &>::value)
            : _base_type(base_iterator._current_row_begin,
                         base_iterator._offset_between_rows) { }

        constexpr matrix_rows_reverse_iterator(BaseIteratorType && base_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   decltype(::gtfo::move(_tt::declval<BaseIteratorType &>()._current_row_begin)),
                                                   decltype(::gtfo::move(_tt::declval<BaseIteratorType &>()._offset_between_rows))>::value)
            : _base_type(::gtfo::move(base_iterator._current_row_begin),
                         ::gtfo::move(base_iterator._offset_between_rows)) { }

    public:
        constexpr BaseIteratorType base() const
            noexcept(_tt::is_nothrow_constructible<BaseIteratorType,
                                                   decltype(_tt::declval<const matrix_rows_reverse_iterator &>()._current_row_begin.base()),
                                                   const decltype(_tt::declval<const matrix_rows_reverse_iterator &>()._offset_between_rows) &>::value &&
                     _tt::is_nothrow_move_constructible<BaseIteratorType>::value)
        {
            return BaseIteratorType(this->_current_row_begin.base(),
                                    this->_offset_between_rows);
        }

    public:
        GTFO_CONSTEXPR_FUNCTION matrix_rows_reverse_iterator & operator = (const matrix_rows_reverse_iterator & other)
            noexcept(_tt::is_nothrow_copy_assignable<_base_type>::value)
        {
            _base_type::operator=(static_cast<const _base_type &>(other));
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_reverse_iterator & operator = (matrix_rows_reverse_iterator && other)
            noexcept(_tt::is_nothrow_move_assignable<_base_type>::value)
        {
            _base_type::operator=(::gtfo::move(static_cast<_base_type &&>(other)));
            return *this;
        }
    };

    template<typename UnderlyingRowElementsIterator, typename DifferenceType, typename NonConstReverseIteratorType, typename BaseIteratorType>
    class matrix_rows_const_reverse_iterator
        : public matrix_rows_iterator_base<
                     UnderlyingRowElementsIterator, DifferenceType,
                     matrix_rows_const_reverse_iterator<UnderlyingRowElementsIterator, DifferenceType, NonConstReverseIteratorType, BaseIteratorType>
                 >
    {
        static_assert(!_tt::is_reference<NonConstReverseIteratorType>::value, "");
        static_assert(!_tt::is_const<NonConstReverseIteratorType>::value, "");
        static_assert(!_tt::is_volatile<NonConstReverseIteratorType>::value, "");
        static_assert(_tt::is_base_of<matrix_rows_iterator_common_base, NonConstReverseIteratorType>::value, "");

        static_assert(!_tt::is_reference<BaseIteratorType>::value, "");
        static_assert(!_tt::is_const<BaseIteratorType>::value, "");
        static_assert(!_tt::is_volatile<BaseIteratorType>::value, "");
        static_assert(_tt::is_base_of<matrix_rows_iterator_common_base, BaseIteratorType>::value, "");

    private:
        using _base_type = matrix_rows_iterator_base<
                               UnderlyingRowElementsIterator, DifferenceType,
                               matrix_rows_const_reverse_iterator<UnderlyingRowElementsIterator, DifferenceType, NonConstReverseIteratorType, BaseIteratorType>
                           >;
    public:
        constexpr matrix_rows_const_reverse_iterator()
            noexcept(_tt::is_nothrow_default_constructible<_base_type>::value)
            : _base_type() { }

        template<typename RowElementsIterator>
        constexpr matrix_rows_const_reverse_iterator(RowElementsIterator && matrix_row_begin, DifferenceType offset_between_rows)
            noexcept(_tt::is_nothrow_constructible<_base_type, RowElementsIterator, DifferenceType>::value)
            : _base_type(::gtfo::forward<RowElementsIterator>(matrix_row_begin),
                         ::gtfo::move(offset_between_rows)) { }

        constexpr matrix_rows_const_reverse_iterator(const matrix_rows_const_reverse_iterator & other)
            noexcept(_tt::is_nothrow_copy_constructible<_base_type>::value)
            : _base_type(static_cast<const _base_type &>(other)) { }

        constexpr matrix_rows_const_reverse_iterator(matrix_rows_const_reverse_iterator && other)
            noexcept(_tt::is_nothrow_move_constructible<_base_type>::value)
            : _base_type(::gtfo::move(static_cast<_base_type &&>(other))) { }

    public:
        constexpr matrix_rows_const_reverse_iterator(const NonConstReverseIteratorType & non_const_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   const decltype(_tt::declval<const NonConstReverseIteratorType &>()._current_row_begin) &,
                                                   const decltype(_tt::declval<const NonConstReverseIteratorType &>()._offset_between_rows) &>::value)
            : _base_type(non_const_iterator._current_row_begin,
                         non_const_iterator._offset_between_rows) { }

        constexpr matrix_rows_const_reverse_iterator(NonConstReverseIteratorType && non_const_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   decltype(::gtfo::move(_tt::declval<NonConstReverseIteratorType &>()._current_row_begin)),
                                                   decltype(::gtfo::move(_tt::declval<NonConstReverseIteratorType &>()._offset_between_rows))>::value)
            : _base_type(::gtfo::move(non_const_iterator._current_row_begin),
                         ::gtfo::move(non_const_iterator._offset_between_rows)) { }


    public:
        constexpr matrix_rows_const_reverse_iterator(const BaseIteratorType & base_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   const decltype(_tt::declval<const BaseIteratorType &>()._current_row_begin) &,
                                                   const decltype(_tt::declval<const BaseIteratorType &>()._offset_between_rows) &>::value)
            : _base_type(base_iterator._current_row_begin,
                         base_iterator._offset_between_rows) { }

        constexpr matrix_rows_const_reverse_iterator(BaseIteratorType && base_iterator)
            noexcept(_tt::is_nothrow_constructible<_base_type,
                                                   decltype(::gtfo::move(_tt::declval<BaseIteratorType &>()._current_row_begin)),
                                                   decltype(::gtfo::move(_tt::declval<BaseIteratorType &>()._offset_between_rows))>::value)
            : _base_type(::gtfo::move(base_iterator._current_row_begin),
                         ::gtfo::move(base_iterator._offset_between_rows)) { }

    public:
        constexpr BaseIteratorType base() const
            noexcept(_tt::is_nothrow_constructible<BaseIteratorType,
                                                   decltype(_tt::declval<const matrix_rows_const_reverse_iterator &>()._current_row_begin.base()),
                                                   const decltype(_tt::declval<const matrix_rows_const_reverse_iterator &>()._offset_between_rows) &>::value &&
                     _tt::is_nothrow_move_constructible<BaseIteratorType>::value)
        {
            return BaseIteratorType(this->_current_row_begin.base(),
                                    this->_offset_between_rows);
        }

    public:
        GTFO_CONSTEXPR_FUNCTION matrix_rows_const_reverse_iterator & operator = (const matrix_rows_const_reverse_iterator & other)
            noexcept(_tt::is_nothrow_copy_assignable<_base_type>::value)
        {
            _base_type::operator=(static_cast<const _base_type &>(other));
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION matrix_rows_const_reverse_iterator & operator = (matrix_rows_const_reverse_iterator && other)
            noexcept(_tt::is_nothrow_move_assignable<_base_type>::value)
        {
            _base_type::operator=(::gtfo::move(static_cast<_base_type &&>(other)));
            return *this;
        }
    };

    template<typename T1, typename T2>
    constexpr
    typename _tt::enable_if<
        _tt::is_base_of<matrix_rows_iterator_common_base, T1>::value &&
        _tt::is_base_of<matrix_rows_iterator_common_base, T2>::value,
        bool
    >::type
        operator == (const T1 & lhs, const T2 & rhs)
        noexcept(noexcept(_tt::declval<const T1 &>()._current_row_begin == _tt::declval<const T2 &>()._current_row_begin))
    {
        GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
        return lhs._current_row_begin == rhs._current_row_begin;
    }

    template<typename T1, typename T2>
    constexpr
    typename _tt::enable_if<
        _tt::is_base_of<matrix_rows_iterator_common_base, T1>::value &&
        _tt::is_base_of<matrix_rows_iterator_common_base, T2>::value,
        bool
    >::type
        operator != (const T1 & lhs, const T2 & rhs)
        noexcept(noexcept(_tt::declval<const T1 &>()._current_row_begin != _tt::declval<const T2 &>()._current_row_begin))
    {
        GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
        return lhs._current_row_begin != rhs._current_row_begin;
    }

    template<typename T1, typename T2>
    constexpr
    typename _tt::enable_if<
        _tt::is_base_of<matrix_rows_iterator_common_base, T1>::value &&
        _tt::is_base_of<matrix_rows_iterator_common_base, T2>::value,
        bool
    >::type
        operator < (const T1 & lhs, const T2 & rhs)
        noexcept(noexcept(_tt::declval<const T1 &>()._current_row_begin < _tt::declval<const T2 &>()._current_row_begin))
    {
        GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
        return lhs._current_row_begin < rhs._current_row_begin;
    }

    template<typename T1, typename T2>
    constexpr
    typename _tt::enable_if<
        _tt::is_base_of<matrix_rows_iterator_common_base, T1>::value &&
        _tt::is_base_of<matrix_rows_iterator_common_base, T2>::value,
        bool
    >::type
        operator > (const T1 & lhs, const T2 & rhs)
        noexcept(noexcept(_tt::declval<const T1 &>()._current_row_begin > _tt::declval<const T2 &>()._current_row_begin))
    {
        GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
        return lhs._current_row_begin > rhs._current_row_begin;
    }

    template<typename T1, typename T2>
    constexpr
    typename _tt::enable_if<
        _tt::is_base_of<matrix_rows_iterator_common_base, T1>::value &&
        _tt::is_base_of<matrix_rows_iterator_common_base, T2>::value,
        bool
    >::type
        operator <= (const T1 & lhs, const T2 & rhs)
        noexcept(noexcept(_tt::declval<const T1 &>()._current_row_begin <= _tt::declval<const T2 &>()._current_row_begin))
    {
        GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
        return lhs._current_row_begin <= rhs._current_row_begin;
    }

    template<typename T1, typename T2>
    constexpr
    typename _tt::enable_if<
        _tt::is_base_of<matrix_rows_iterator_common_base, T1>::value &&
        _tt::is_base_of<matrix_rows_iterator_common_base, T2>::value,
        bool
    >::type
        operator >= (const T1 & lhs, const T2 & rhs)
        noexcept(noexcept(_tt::declval<const T1 &>()._current_row_begin >= _tt::declval<const T2 &>()._current_row_begin))
    {
        GTFO_DEBUG_ASSERT(lhs._offset_between_rows == rhs._offset_between_rows);
        return lhs._current_row_begin >= rhs._current_row_begin;
    }
}
}
}

#endif // MATRIX_UTILS_MATRIX_ROWS_ITERATOR_HPP_INCLUDED
