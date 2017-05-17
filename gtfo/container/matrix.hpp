#ifndef GTFO_FILE_INCLUDED_CONTAINER_MATRIX_HPP
#define GTFO_FILE_INCLUDED_CONTAINER_MATRIX_HPP

#include "gtfo/_impl/iterator_range.hpp"
#include "gtfo/_impl/container/matrix/matrix_rows_iterator.hpp"
#include "gtfo/_impl/container/matrix/initializer_list_utils.hpp"
#include "gtfo/_impl/container/matrix/matrix_exceptions.hpp"

#include <vector>
#include <algorithm>

namespace gtfo
{
inline namespace container
{

template<typename T, typename UnderlyingVector = ::std::vector<T>>
class matrix
{
    static_assert(_tt::is_same<T, typename UnderlyingVector::value_type>::value, "invalid types specified");

private:
    using underlying_vector      = UnderlyingVector;

public:
    using size_type              = typename underlying_vector::size_type;
    using difference_type        = typename underlying_vector::difference_type;
    using value_type             = typename underlying_vector::value_type;
    using allocator_type         = typename underlying_vector::allocator_type;
    using reference              = typename underlying_vector::reference;
    using const_reference        = typename underlying_vector::const_reference;
    using pointer                = typename underlying_vector::pointer;
    using const_pointer          = typename underlying_vector::const_pointer;

private:
    using matrix_rows_const_iterator         = detail::container::matrix_rows_iterator<typename underlying_vector::const_iterator,         difference_type, void,                               void>;
    using matrix_rows_iterator               = detail::container::matrix_rows_iterator<typename underlying_vector::iterator,               difference_type, matrix_rows_const_iterator,         void>;
    using matrix_rows_const_reverse_iterator = detail::container::matrix_rows_iterator<typename underlying_vector::const_reverse_iterator, difference_type, void,                               matrix_rows_const_iterator>;
    using matrix_rows_reverse_iterator       = detail::container::matrix_rows_iterator<typename underlying_vector::reverse_iterator,       difference_type, matrix_rows_const_reverse_iterator, matrix_rows_iterator>;

public:
    using iterator               = matrix_rows_iterator;
    using const_iterator         = matrix_rows_const_iterator;
    using reverse_iterator       = matrix_rows_reverse_iterator;
    using const_reverse_iterator = matrix_rows_const_reverse_iterator;

private:
    using initializer_list_1d    = ::std::initializer_list<value_type>;
    using initializer_list_2d    = ::std::initializer_list<initializer_list_1d>;

private:
    underlying_vector            _data;
    size_type                    _cols;
    size_type                    _rows;

    static_assert                (_tt::is_nothrow_default_constructible<size_type>::value, "");
    static_assert                (_tt::is_nothrow_copy_constructible<size_type>::value, "");
    static_assert                (_tt::is_nothrow_move_constructible<size_type>::value, "");
    static_assert                (_tt::is_nothrow_constructible<size_type, int>::value, "");
    static_assert                (_tt::is_nothrow_copy_assignable<size_type>::value, "");
    static_assert                (_tt::is_nothrow_move_assignable<size_type>::value, "");
    static_assert                (noexcept(_tt::declval<const size_type &>() == _tt::declval<const size_type &>()), "");
    static_assert                (noexcept(_tt::declval<const size_type &>() != _tt::declval<const size_type &>()), "");
    static_assert                (noexcept(_tt::declval<const size_type &>() <  _tt::declval<const size_type &>()), "");
    static_assert                (noexcept(_tt::declval<const size_type &>() <= _tt::declval<const size_type &>()), "");
    static_assert                (noexcept(_tt::declval<const size_type &>() >  _tt::declval<const size_type &>()), "");
    static_assert                (noexcept(_tt::declval<const size_type &>() >= _tt::declval<const size_type &>()), "");

    static_assert                (_tt::is_nothrow_default_constructible<difference_type>::value, "");
    static_assert                (_tt::is_nothrow_copy_constructible<difference_type>::value, "");
    static_assert                (_tt::is_nothrow_move_constructible<difference_type>::value, "");
    static_assert                (_tt::is_nothrow_constructible<difference_type, int>::value, "");
    static_assert                (_tt::is_nothrow_copy_assignable<difference_type>::value, "");
    static_assert                (_tt::is_nothrow_move_assignable<difference_type>::value, "");
    static_assert                (noexcept(_tt::declval<const difference_type &>() == _tt::declval<const difference_type &>()), "");
    static_assert                (noexcept(_tt::declval<const difference_type &>() != _tt::declval<const difference_type &>()), "");
    static_assert                (noexcept(_tt::declval<const difference_type &>() <  _tt::declval<const difference_type &>()), "");
    static_assert                (noexcept(_tt::declval<const difference_type &>() <= _tt::declval<const difference_type &>()), "");
    static_assert                (noexcept(_tt::declval<const difference_type &>() >  _tt::declval<const difference_type &>()), "");
    static_assert                (noexcept(_tt::declval<const difference_type &>() >= _tt::declval<const difference_type &>()), "");

private:
    constexpr void _invariant() const noexcept
    {
        GTFO_DEBUG_ASSERT        (bool(_cols == 0 || _rows == 0) == bool(_data.empty()));
        GTFO_DEBUG_ASSERT        (_data.size() == _rows * _cols);
        GTFO_DEBUG_ASSERT        ((_cols == 0) || (_data.size() % _cols == 0));
        GTFO_DEBUG_ASSERT        ((_cols == 0) || (_data.size() / _cols == _rows));
        GTFO_DEBUG_ASSERT        ((_rows == 0) || (_data.size() % _rows == 0));
        GTFO_DEBUG_ASSERT        ((_rows == 0) || (_data.size() / _rows == _cols));
    }

public: // constructors
             matrix              ()
                                     noexcept(_tt::is_nothrow_default_constructible<underlying_vector>::value)
                                 : _data()
                                 , _cols(0)
                                 , _rows(0) { _invariant(); }

    explicit matrix              (const allocator_type & alloc)
                                     noexcept(_tt::is_nothrow_constructible<underlying_vector, const allocator_type &>::value)
                                 : _data(alloc)
                                 , _cols(0)
                                 , _rows(0) { _invariant(); }

             matrix              (size_type rows, size_type cols, const allocator_type & alloc = allocator_type())
                                 : _data(rows * cols, alloc)
                                 , _cols(cols)
                                 , _rows(rows) { _invariant(); }

             matrix              (size_type rows, size_type cols, const value_type & val, const allocator_type & alloc = allocator_type())
                                 : _data(rows * cols, val, alloc)
                                 , _cols(cols)
                                 , _rows(rows) { _invariant(); }

             matrix              (const matrix & other)
                                 : _data(other._data)
                                 , _cols(other._cols)
                                 , _rows(other._rows) { _invariant(); }

             matrix              (const matrix & other, const allocator_type & alloc)
                                 : _data(other._data, alloc)
                                 , _cols(other._cols)
                                 , _rows(other._rows) { _invariant(); }

             matrix              (matrix && other)
                                     noexcept(_tt::is_nothrow_move_constructible<underlying_vector>::value)
                                 : _data(::gtfo::move(other._data))
                                 , _cols(::gtfo::move(other._cols))
                                 , _rows(::gtfo::move(other._rows)) { _invariant(); }

             matrix              (matrix && other, const allocator_type & alloc)
                                     noexcept(_tt::is_nothrow_constructible<underlying_vector, underlying_vector &&, const allocator_type &>::value)
                                 : _data(::gtfo::move(other._data), alloc)
                                 , _cols(::gtfo::move(other._cols))
                                 , _rows(::gtfo::move(other._rows)) { _invariant(); }

             matrix              (initializer_list_2d il_2d, const allocator_type & alloc = allocator_type())
                                 : _data(detail::get_2d_initializer_list_total_size(il_2d), value_type(), alloc)
                                 , _rows(static_cast<size_type>(il_2d.size()))
                                 {
                                     using il_2d_size_type = typename initializer_list_2d::size_type;
                                     const il_2d_size_type cols_result = detail::get_2d_initializer_list_num_cols(il_2d);
                                     if (cols_result == il_2d_size_type(-1))
                                         throw bad_matrix_initializer_list();
                                     _cols = static_cast<size_type>(cols_result);
                                     _invariant();
                                     auto my_it = _data.begin();
                                     for (const auto & sub_il_1d : il_2d) {
                                         for (const auto & val : sub_il_1d)
                                             *my_it++ = val;
                                     }
                                 }

public: // assignment operator
    matrix & operator = (matrix && other)
        noexcept(_tt::is_nothrow_move_assignable<underlying_vector>::value)
    {
        _data = ::gtfo::move(other._data);
        _cols = ::gtfo::move(other._cols);
        _rows = ::gtfo::move(other._rows);
        return *this;
    }

    matrix & operator = (const matrix & other)
    {
        matrix tmp(other);
        *this = ::gtfo::move(tmp);
        _invariant();
        return *this;
    }

    matrix & operator = (initializer_list_2d il)
    {
        matrix tmp(il);
        *this = ::gtfo::move(tmp);
        _invariant();
        return *this;
    }

public: // swap
    void swap(matrix & other)
        noexcept(noexcept(_tt::declval<underlying_vector &>().swap(_tt::declval<underlying_vector &>())))
    {
        _data.swap(other._data);
        ::gtfo::swap(_cols, other._cols);
        ::gtfo::swap(_rows, other._rows);
        _invariant();
        other._invariant();
    }

    friend inline void swap(matrix & lhs, matrix & rhs)
        noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

public: // assign
    matrix & assign(size_type rows, size_type cols, const value_type & value)
    {
        _data.resize(rows * cols, value);
        _cols = ::gtfo::move(cols);
        _rows = ::gtfo::move(rows);
        return *this;
    }

    matrix & assign(initializer_list_2d il)
    {
        return (*this = il);
    }

public: // get_allocator
    allocator_type get_allocator() const
        noexcept(noexcept(_tt::declval<const underlying_vector &>().get_allocator()) &&
                 _tt::is_nothrow_move_constructible<allocator_type>::value)
    {
        return _data.get_allocator();
    }

public: // raw access to underlying data
    value_type * underlying_data() noexcept
    {
        return _data.data();
    }

    const value_type * underlying_data() const noexcept
    {
        return _data.data();
    }

public: // size and capacity
    size_type rows() const noexcept
    {
        return _rows;
    }

    size_type cols() const noexcept
    {
        return _cols;
    }

    size_type overall_size() const noexcept
    {
        return _data.size();
    }

    size_type overall_max_size() const noexcept
    {
        return _data.max_size();
    }

    size_type overall_capacity() const noexcept
    {
        return _data.capacity();
    }

    void reserve_rows(size_type rows)
    {
        _data.reserve(rows * _cols);
    }

    void reserve_cols(size_type cols)
    {
        _data.reserve(_rows * cols);
    }

    void reserve_overall(size_type overall_size)
    {
        _data.reserve(overall_size);
    }

    void shrink_to_fit()
    {
        _data.shrink_to_fit();
    }

    void clear() noexcept
    {
        _data.clear();
        _cols = size_type(0);
        _rows = size_type(0);
        _invariant();
    }

    void clear_rows() noexcept // remember _cols
    {
        _data.clear();
        _rows = size_type(0);
        _invariant();
    }

    void clear_cols() noexcept // remember _rows
    {
        _data.clear();
        _cols = size_type(0);
        _invariant();
    }

private:
    // if the caller function has nothing more to do, returns 0;
    // otherwise returns number of elements that must be appended by the caller
    size_type _resize_rows_common_part(size_type new_num_rows)
    {
        if (_cols == 0) {
            GTFO_DEBUG_ASSERT(_data.empty());
            _rows = new_num_rows;
            return size_type(0);
        }

        const size_type old_num_rows = _rows;
        if (new_num_rows == old_num_rows)
            return size_type(0);
        if (new_num_rows == 0) {
            this->clear_rows();
            return size_type(0);
        }

        if (new_num_rows < old_num_rows) {
            const size_type num_removed_elements = (old_num_rows - new_num_rows) * _cols;
            _data.erase(_data.end() - num_removed_elements, _data.end());
            _rows = new_num_rows;
            return size_type(0);
        }

        return (new_num_rows - old_num_rows) * _cols;
    }

public:
    iterator erase_row(const_iterator position)
    {
        const size_type removed_row_index = position - this->cbegin();
        if (removed_row_index == _rows)
            return this->begin() + removed_row_index;
        GTFO_DEBUG_ASSERT(_rows > 0);
        GTFO_DEBUG_ASSERT(position < this->cend());
        ::gtfo::iterator_range<typename underlying_vector::const_iterator> removed_range(*position);
        _data.erase(removed_range.begin(), removed_range.end());
        --_rows;
        _invariant();
        return this->begin() + removed_row_index;
    }

    iterator erase_rows(const_iterator pos_rows_begin, const_iterator pos_rows_end)
    {
        const size_type first_removed_row_index = pos_rows_begin - this->cbegin();
        if (pos_rows_begin == pos_rows_end)
            return this->begin() + first_removed_row_index;
        GTFO_DEBUG_ASSERT(_rows > 0);
        GTFO_DEBUG_ASSERT(pos_rows_begin < pos_rows_end);
        GTFO_DEBUG_ASSERT(pos_rows_end <= this->cend());
        const size_type num_removed_rows(pos_rows_end - pos_rows_begin);
        ::gtfo::iterator_range<typename underlying_vector::const_iterator> first_removed_range(*pos_rows_begin);
        --pos_rows_end;
        ::gtfo::iterator_range<typename underlying_vector::const_iterator> last_removed_range(*pos_rows_end);
        _data.erase(first_removed_range.begin(), last_removed_range.end());
        _rows -= num_removed_rows;
        _invariant();
        return this->begin() + first_removed_row_index;
    }

    void resize_rows(size_type new_num_rows)
    {
        const size_type num_appended_elements = this->_resize_rows_common_part(new_num_rows);
        if (num_appended_elements > 0) {
            _data.insert(_data.end(), num_appended_elements, value_type());
            _rows = new_num_rows;
        }
        _invariant();
        GTFO_DEBUG_ASSERT(_rows == new_num_rows);
    }

    void resize_rows(size_type new_num_rows, const value_type & filler_element)
    {
        const size_type num_appended_elements = this->_resize_rows_common_part(new_num_rows);
        if (num_appended_elements > 0) {
            _data.insert(_data.end(), num_appended_elements, filler_element);
            _rows = new_num_rows;
        }
        _invariant();
        GTFO_DEBUG_ASSERT(_rows == new_num_rows);
    }

    template<typename FillerRowIterator>
    void resize_rows(size_type new_num_rows, FillerRowIterator filler_row_begin, FillerRowIterator filler_row_end)
    {
        const size_type num_appended_elements = this->_resize_rows_common_part(new_num_rows);
        if (num_appended_elements > 0) {
            GTFO_DEBUG_ASSERT(num_appended_elements % _cols == 0);
            if (static_cast<size_type>(::std::distance(filler_row_begin, filler_row_end)) != _cols)
                throw bad_matrix_filler_row();
            _data.reserve(_data.size() + num_appended_elements);
            for (size_type iteration(0), num_iterations(num_appended_elements / _cols); iteration < num_iterations; ++iteration) {
                _data.insert(_data.end(), filler_row_begin, filler_row_end);
            }
            _rows = new_num_rows;
        }
        _invariant();
        GTFO_DEBUG_ASSERT(_rows == new_num_rows);
    }

    void resize_rows(size_type new_num_rows, initializer_list_1d filler_row_il)
    {
        this->resize_rows(new_num_rows, filler_row_il.begin(), filler_row_il.end());
    }

private:
    void _lshift_data_elements(size_type i_dest, size_type i_begin, size_type i_end)
    {
        GTFO_DEBUG_ASSERT(i_dest < i_begin);
        GTFO_DEBUG_ASSERT(i_begin < i_end);
        GTFO_DEBUG_ASSERT(i_end <= _data.size());
        while (i_begin != i_end) {
            _data[i_dest] = ::gtfo::move(_data[i_begin]);
            ++i_dest;
            ++i_begin;
        }
    }

    void _remove_last_n_columns(size_type num_removed_columns) // does not change _cols, just shifts the data
    {
        GTFO_DEBUG_ASSERT(_rows > 0 && _cols > 0 && _data.size() > 0);
        GTFO_DEBUG_ASSERT(num_removed_columns > 0);
        GTFO_DEBUG_ASSERT(num_removed_columns < _cols);
        const size_type future_num_cols     (_cols - num_removed_columns);
        size_type removed_data_element_index(future_num_cols);
        for (size_type i_row(1); i_row < _rows; ++i_row) {
            size_type current_row_old_i_begin(i_row * _cols);
            size_type current_row_old_i_end  (current_row_old_i_begin + _cols);
            _lshift_data_elements            (removed_data_element_index, current_row_old_i_begin, current_row_old_i_end);
            removed_data_element_index       += future_num_cols;
        }
    }

    void _rshift_data_elements(size_type i_rev_dest, size_type i_rev_begin, size_type i_rev_end)
    {
        GTFO_DEBUG_ASSERT(i_rev_end <= i_rev_begin);
        GTFO_DEBUG_ASSERT(i_rev_dest > i_rev_begin);
        GTFO_DEBUG_ASSERT(i_rev_dest <= _data.size());
        while (i_rev_begin != i_rev_end) {
            _data[i_rev_dest] = ::gtfo::move(_data[i_rev_begin]);
            --i_rev_dest;
            --i_rev_begin;
        }
    }

    void _append_new_n_columns(size_type num_added_columns) // does not change _cols, just shifts the data
    {
        GTFO_DEBUG_ASSERT(_rows > 0 && _cols > 0 && _data.size() > 0);
        GTFO_DEBUG_ASSERT(num_added_columns > 0);
        GTFO_DEBUG_ASSERT(_data.size() == _rows * (_cols + num_added_columns));
        for (size_type i_row(_rows - 1); i_row > 0; --i_row) { // row 0 should not be shifted anywhere
            size_type current_row_old_i_rev_end  (i_row * _cols - 1);
            size_type current_row_old_i_rev_begin(current_row_old_i_rev_end + _cols);
            size_type current_row_new_i_rev_begin(current_row_old_i_rev_end + (_cols + num_added_columns));
            GTFO_DEBUG_ASSERT(current_row_new_i_rev_begin > current_row_old_i_rev_begin);
            _rshift_data_elements                (current_row_new_i_rev_begin, current_row_old_i_rev_begin, current_row_old_i_rev_end);
        }
    }

    // if the caller function has nothing more to do, returns 0;
    // otherwise returns number of elements that must be appended by the caller
    size_type _resize_cols_common_part(size_type new_num_cols)
    {
        if (_rows == 0) {
            GTFO_DEBUG_ASSERT(_data.empty());
            _cols = new_num_cols;
            return size_type(0);
        }

        const size_type old_num_cols = _cols;
        if (new_num_cols == old_num_cols)
            return size_type(0);
        if (new_num_cols == 0) {
            this->clear_cols();
            return size_type(0);
        }

        if (new_num_cols < old_num_cols) {
            const size_type num_removed_columns = old_num_cols - new_num_cols;
            const size_type num_removed_elements = num_removed_columns * _rows;
            _remove_last_n_columns(num_removed_columns);
            _data.erase(_data.end() - num_removed_elements, _data.end());
            _cols = new_num_cols;
            return size_type(0);
        }

        return (new_num_cols - old_num_cols) * _rows;
    }

public:
    void resize_cols(size_type new_num_cols)
    {
        size_type num_added_elements = _resize_cols_common_part(new_num_cols);
        if (num_added_elements) {
            GTFO_DEBUG_ASSERT(new_num_cols > _cols);
            GTFO_DEBUG_ASSERT(num_added_elements == _rows * (new_num_cols - _cols));
            _data.resize(_data.size() + num_added_elements);
            _append_new_n_columns(new_num_cols - _cols);
            for (size_type i_row(0); i_row < _rows; ++i_row) {
                for (size_type i_col(_cols); i_col < new_num_cols; ++i_col)
                    _data[i_row * new_num_cols + i_col] = value_type();
            }
            _cols = new_num_cols;
            // TODO : implement appending
        }
        _invariant();
    }

public: // at
    const_reference at(size_type row, size_type col) const
    {
        if (row >= _rows)
            throw bad_matrix_row_index();
        if (col >= _cols)
            throw bad_matrix_column_index();
        return _data[row * _cols + col];
    }

    reference at(size_type row, size_type col)
    {
        return const_cast<reference>(const_cast<const matrix &>(*this).at(row, col));
    }

public: // (unchecked) raw_at
    const_reference raw_at(size_type row, size_type col) const
    {
        GTFO_DEBUG_ASSERT(row < _rows);
        GTFO_DEBUG_ASSERT(col < _cols);
        return _data[row * _cols + col];
    }

    reference raw_at(size_type row, size_type col)
    {
        return const_cast<reference>(const_cast<const matrix &>(*this).raw_at(row, col));
    }

public: // row iterators
    matrix_rows_iterator begin()
    {
        return matrix_rows_iterator(_data.begin(), static_cast<difference_type>(_cols));
    }

    matrix_rows_iterator end()
    {
        return matrix_rows_iterator(_data.end(), static_cast<difference_type>(_cols));
    }

public: // const row iterators
    matrix_rows_const_iterator begin() const
    {
        return matrix_rows_const_iterator(_data.cbegin(), static_cast<difference_type>(_cols));
    }

    matrix_rows_const_iterator end() const
    {
        return matrix_rows_const_iterator(_data.cend(), static_cast<difference_type>(_cols));
    }

    matrix_rows_const_iterator cbegin() const
    {
        return this->begin();
    }

    matrix_rows_const_iterator cend() const
    {
        return this->end();
    }

public: // reverse row iterators
    matrix_rows_reverse_iterator rbegin()
    {
        return matrix_rows_reverse_iterator(_data.rbegin(), static_cast<difference_type>(_cols));
    }

    matrix_rows_reverse_iterator rend()
    {
        return matrix_rows_reverse_iterator(_data.rend(), static_cast<difference_type>(_cols));
    }

public: // const reverse row iterators
    matrix_rows_const_reverse_iterator rbegin() const
    {
        return matrix_rows_const_reverse_iterator(_data.crbegin(), static_cast<difference_type>(_cols));
    }

    matrix_rows_const_reverse_iterator rend() const
    {
        return matrix_rows_const_reverse_iterator(_data.crend(), static_cast<difference_type>(_cols));
    }

    matrix_rows_const_reverse_iterator crbegin() const
    {
        return this->rbegin();
    }

    matrix_rows_const_reverse_iterator crend() const
    {
        return this->rend();
    }

public:
    friend inline bool operator == (const matrix & lhs, const matrix & rhs)
        noexcept(noexcept(_tt::declval<const underlying_vector &>() == _tt::declval<const underlying_vector &>()))
    {
        if (lhs._rows != rhs._rows || lhs._cols != rhs._cols)
            return false;
        return lhs._data == rhs._data;
    }

    friend inline bool operator != (const matrix & lhs, const matrix & rhs)
        noexcept(noexcept(_tt::declval<const underlying_vector &>() != _tt::declval<const underlying_vector &>()))
    {
        if (lhs._rows != rhs._rows || lhs._cols != rhs._cols)
            return true;
        return lhs._data != rhs._data;
    }
};

}
}

#endif // GTFO_FILE_INCLUDED_CONTAINER_MATRIX_HPP
