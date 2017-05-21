#ifndef GTFO_FILE_INCLUDED_ITERATOR_RANGE_HPP
#define GTFO_FILE_INCLUDED_ITERATOR_RANGE_HPP

#include "gtfo/_impl/utility.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#include "gtfo/_impl/type_traits/are_comparable_op_n_eq.hpp"

namespace gtfo
{
    template<typename BeginIterator, typename EndIterator = BeginIterator>
    class iterator_range
    {
        static_assert(_tt::are_comparable_op_eq<BeginIterator, EndIterator>::value &&
                      _tt::are_comparable_op_n_eq<BeginIterator, EndIterator>::value,
                      "BeginIterator and EndIterator must be comparable with both "
                      "operator == and operator !=");

    public:
        using begin_iterator_type = BeginIterator;
        using end_iterator_type   = EndIterator;

    private:
        begin_iterator_type _begin_iterator;
        end_iterator_type   _end_iterator;

    public:
        constexpr iterator_range(begin_iterator_type begin_iterator, end_iterator_type end_iterator)
                                    noexcept(_tt::is_nothrow_copy_constructible<begin_iterator_type>::value &&
                                             _tt::is_nothrow_move_constructible<begin_iterator_type>::value &&
                                             _tt::is_nothrow_copy_constructible<end_iterator_type>::value &&
                                             _tt::is_nothrow_move_constructible<end_iterator_type>::value)
                                : _begin_iterator (::gtfo::move(begin_iterator))
                                , _end_iterator   (::gtfo::move(end_iterator)) { }

        constexpr iterator_range(const iterator_range & other)
                                    noexcept(_tt::is_nothrow_copy_constructible<begin_iterator_type>::value &&
                                             _tt::is_nothrow_copy_constructible<end_iterator_type>::value)
                                : _begin_iterator (other._begin_iterator)
                                , _end_iterator   (other._end_iterator) { }

        constexpr iterator_range(iterator_range && other)
                                    noexcept(_tt::is_nothrow_move_constructible<begin_iterator_type>::value &&
                                             _tt::is_nothrow_move_constructible<end_iterator_type>::value)
                                : _begin_iterator (::gtfo::move(other._begin_iterator))
                                , _end_iterator   (::gtfo::move(other._end_iterator)) { }

    public:
        GTFO_CONSTEXPR_FUNCTION iterator_range & operator = (const iterator_range & other)
            noexcept(_tt::is_nothrow_copy_assignable<begin_iterator_type>::value &&
                     _tt::is_nothrow_copy_assignable<end_iterator_type>::value)
        {
            _begin_iterator = other._begin_iterator;
            _end_iterator   = other._end_iterator;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION iterator_range & operator = (iterator_range && other)
            noexcept(_tt::is_nothrow_move_assignable<begin_iterator_type>::value &&
                     _tt::is_nothrow_move_assignable<end_iterator_type>::value)
        {
            _begin_iterator = ::gtfo::move(other._begin_iterator);
            _end_iterator   = ::gtfo::move(other._end_iterator);
            return *this;
        }

        constexpr begin_iterator_type begin() const
            noexcept(_tt::is_nothrow_copy_constructible<begin_iterator_type>::value)
        {
            return _begin_iterator;
        }

        constexpr end_iterator_type end() const
            noexcept(_tt::is_nothrow_copy_constructible<end_iterator_type>::value)
        {
            return _end_iterator;
        }

        GTFO_CONSTEXPR_FUNCTION
            typename _tt::enable_if<
                         _tt::is_same<
                             typename iterator_traits<begin_iterator_type>::iterator_category,
                             random_access_iterator_tag
                         >::value,
                         decltype( _tt::declval<begin_iterator_type &>() [ _tt::declval<typename iterator_traits<begin_iterator_type>::difference_type &>() ] )
                     >::type
            operator [] (typename iterator_traits<begin_iterator_type>::difference_type index)
        {
            GTFO_DEBUG_ASSERT((index >= 0) && (index < (_end_iterator - _begin_iterator)));
            return _begin_iterator[index];
        }

        constexpr
            typename _tt::enable_if<
                         _tt::is_same<
                             typename iterator_traits<begin_iterator_type>::iterator_category,
                             random_access_iterator_tag
                         >::value,
                         decltype( _tt::declval<const begin_iterator_type &>() [ _tt::declval<typename iterator_traits<begin_iterator_type>::difference_type &>() ] )
                     >::type
            operator [] (typename iterator_traits<begin_iterator_type>::difference_type index) const
        {
            GTFO_DEBUG_ASSERT((index >= 0) && (index < (_end_iterator - _begin_iterator)));
            return _begin_iterator[index];
        }

        friend constexpr begin_iterator_type begin(const iterator_range & r)
            noexcept(_tt::is_nothrow_copy_constructible<begin_iterator_type>::value)
        {
            return r._begin_iterator;
        }

        friend constexpr end_iterator_type end(const iterator_range & r)
            noexcept(_tt::is_nothrow_copy_constructible<end_iterator_type>::value)
        {
            return r._end_iterator;
        }
    };
}

#endif // GTFO_FILE_INCLUDED_ITERATOR_RANGE_HPP
