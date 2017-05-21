#ifndef GTFO_FILE_INCLUDED_ITERATOR_REVERSE_ITERATOR_HPP
#define GTFO_FILE_INCLUDED_ITERATOR_REVERSE_ITERATOR_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    namespace detail
    {
        template<bool iterator_is_pointer>
        struct iterator_op_arrow_impl
        {
            template<typename I>
            static constexpr I apply(I arg) noexcept
            {
                static_assert(_tt::is_pointer<I>::value, "pointer type expected");
                return arg;
            }
        };

        template<>
        struct iterator_op_arrow_impl<false>
        {
            template<typename I>
            static constexpr auto apply(I && arg)
                noexcept(noexcept(_tt::declval<I>().operator->()))
                -> decltype(_tt::declval<I>().operator->())
            {
                static_assert(!_tt::is_pointer<_tt::remove_reference_t<I>>::value, "non-pointer type expected");
                return ::gtfo::forward<I>(arg).operator->();
            }
        };
    }

    template<typename WrappedIterator>
    class reverse_iterator
    {
    private:
        WrappedIterator _wrapped;

    public:
        using iterator_type = WrappedIterator;

        using difference_type = _tt::decay_t<decltype( _tt::declval<const iterator_type &>() - _tt::declval<const iterator_type &>() )>;
        using pointer   = decltype( detail::iterator_op_arrow_impl<_tt::is_pointer<iterator_type>::value>::apply(_tt::declval<iterator_type>()) );
        using reference = decltype( * _tt::declval<const iterator_type &>() );
        using value_type = _tt::decay_t<reference>;
        using iterator_category = typename iterator_traits<iterator_type>::iterator_category;

        constexpr reverse_iterator()
            noexcept(_tt::is_nothrow_default_constructible<iterator_type>::value)
            : _wrapped()
        {
        }

        constexpr explicit reverse_iterator(const iterator_type & other)
            noexcept(_tt::is_nothrow_copy_constructible<iterator_type>::value)
            : _wrapped(other)
        {
        }

        constexpr explicit reverse_iterator(iterator_type && other)
            noexcept(_tt::is_nothrow_move_constructible<iterator_type>::value)
            : _wrapped(::gtfo::move(other))
        {
        }

        template<typename OtherBase>
        constexpr reverse_iterator(const reverse_iterator<OtherBase> & other)
            noexcept(_tt::is_nothrow_constructible<iterator_type, decltype(_tt::declval<const reverse_iterator<OtherBase> &>().base())>::value)
            : _wrapped(other.base())
        {
        }

        template<typename OtherBase>
        constexpr reverse_iterator(reverse_iterator<OtherBase> && other)
            noexcept(_tt::is_nothrow_constructible<iterator_type, decltype(_tt::declval<reverse_iterator<OtherBase>>().base())>::value)
            : _wrapped(::gtfo::move(other).base())
        {
        }

        template<typename OtherBase>
        GTFO_CONSTEXPR_FUNCTION reverse_iterator & operator = (const reverse_iterator<OtherBase> & other)
            noexcept(_tt::is_nothrow_assignable<iterator_type &, decltype(_tt::declval<const reverse_iterator<OtherBase> &>().base())>::value)
        {
            _wrapped = other.base();
            return *this;
        }

        template<typename OtherBase>
        GTFO_CONSTEXPR_FUNCTION reverse_iterator & operator = (reverse_iterator<OtherBase> && other)
            noexcept(_tt::is_nothrow_assignable<iterator_type &, decltype(_tt::declval<reverse_iterator<OtherBase>>().base())>::value)
        {
            _wrapped = ::gtfo::move(other).base();
            return *this;
        }

        constexpr iterator_type base() const &
            noexcept(_tt::is_nothrow_copy_constructible<iterator_type>::value)
        {
            return _wrapped;
        }

        GTFO_CONSTEXPR_FUNCTION iterator_type base() &&
            noexcept(_tt::is_nothrow_move_constructible<iterator_type>::value)
        {
            return ::gtfo::move(_wrapped);
        }

        GTFO_CONSTEXPR_FUNCTION reference operator * () const
            noexcept(_tt::is_nothrow_copy_constructible<iterator_type>::value &&
                     noexcept( -- _tt::declval<iterator_type &>() ) &&
                     noexcept( * _tt::declval<iterator_type &>() ) &&
                     _tt::is_nothrow_move_constructible<reference>::value)
        {
            iterator_type it = _wrapped;
            --it;
            return *it;
        }

        GTFO_CONSTEXPR_FUNCTION pointer operator -> () const
            noexcept(_tt::is_nothrow_copy_constructible<iterator_type>::value &&
                     noexcept( -- _tt::declval<iterator_type &>() ) &&
                     noexcept( detail::iterator_op_arrow_impl<_tt::is_pointer<iterator_type>::value>::apply(_tt::declval<iterator_type>()) ))
        {
            iterator_type it = _wrapped;
            --it;
            return detail::iterator_op_arrow_impl<_tt::is_pointer<iterator_type>::value>::apply(::gtfo::move(it));
        }

        GTFO_CONSTEXPR_FUNCTION reverse_iterator & operator ++ ()
            noexcept(noexcept( -- _tt::declval<iterator_type &>() ))
        {
            --_wrapped;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION reverse_iterator operator ++ (int)
            noexcept(_tt::is_nothrow_copy_constructible<iterator_type>::value &&
                     noexcept( -- _tt::declval<iterator_type &>() ) &&
                     _tt::is_nothrow_move_constructible<iterator_type>::value)
        {
            reverse_iterator temporary = *this;
            --_wrapped;
            return temporary;
        }

        GTFO_CONSTEXPR_FUNCTION reverse_iterator & operator -- ()
            noexcept(noexcept( ++ _tt::declval<iterator_type &>() ))
        {
            ++_wrapped;
            return *this;
        }

        GTFO_CONSTEXPR_FUNCTION reverse_iterator operator -- (int)
            noexcept(_tt::is_nothrow_copy_constructible<iterator_type>::value &&
                     noexcept( ++ _tt::declval<iterator_type &>() ) &&
                     _tt::is_nothrow_move_constructible<iterator_type>::value)
        {
            reverse_iterator temporary = *this;
            ++_wrapped;
            return temporary;
        }

        GTFO_CONSTEXPR_FUNCTION reverse_iterator & operator += (difference_type diff)
            noexcept(noexcept(_tt::declval<iterator_type &>() -= _tt::declval<difference_type &>()))
        {
            _wrapped -= diff;
            return *this;
        }

        constexpr reverse_iterator operator + (difference_type diff) const
            noexcept(noexcept(_tt::declval<const iterator_type &>() - _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_constructible<reverse_iterator, decltype(_tt::declval<const iterator_type &>() - _tt::declval<difference_type &>())>::value &&
                     _tt::is_nothrow_move_constructible<reverse_iterator>::value)
        {
            return reverse_iterator(_wrapped - diff);
        }

        GTFO_CONSTEXPR_FUNCTION reverse_iterator & operator -= (difference_type diff)
            noexcept(noexcept(_tt::declval<iterator_type &>() += _tt::declval<difference_type &>()))
        {
            _wrapped += diff;
            return *this;
        }

        constexpr reverse_iterator operator - (difference_type diff) const
            noexcept(noexcept(_tt::declval<const iterator_type &>() + _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_constructible<reverse_iterator, decltype(_tt::declval<const iterator_type &>() + _tt::declval<difference_type &>())>::value &&
                     _tt::is_nothrow_move_constructible<reverse_iterator>::value)
        {
            return reverse_iterator(_wrapped + diff);
        }

        constexpr reference operator [] (difference_type diff) const
            noexcept(noexcept(_tt::declval<const iterator_type &>() - _tt::declval<difference_type &>()) &&
                     _tt::is_nothrow_constructible<reverse_iterator, decltype(_tt::declval<const iterator_type &>() - _tt::declval<difference_type &>())>::value &&
                     _tt::is_nothrow_move_constructible<reverse_iterator>::value &&
                     noexcept( * _tt::declval<decltype(_tt::declval<const reverse_iterator &>() + _tt::declval<difference_type &>())>() ))
        {
            return *(*this + diff);
        }
    };

    template<typename I>
    constexpr reverse_iterator<I> operator + (typename reverse_iterator<I>::difference_type diff, const reverse_iterator<I> & rit)
        noexcept(noexcept(_tt::declval<const reverse_iterator<I> &>() + _tt::declval<typename reverse_iterator<I>::difference_type &>()) &&
                 _tt::is_nothrow_constructible<reverse_iterator<I>, decltype(_tt::declval<const reverse_iterator<I> &>() + _tt::declval<typename reverse_iterator<I>::difference_type &>())>::value)
    {
        return rit + diff;
    }

    template<typename ILhs, typename IRhs>
    constexpr auto operator - (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<IRhs> &>().base() ) &&
                 noexcept( _tt::declval<const reverse_iterator<ILhs> &>().base() ) &&
                 noexcept( _tt::declval<const reverse_iterator<IRhs> &>().base() - _tt::declval<const reverse_iterator<ILhs> &>().base() ) &&
                 _tt::is_nothrow_move_constructible<decltype(_tt::declval<const reverse_iterator<IRhs> &>().base() - _tt::declval<const reverse_iterator<ILhs> &>().base())>::value)
        -> decltype(_tt::declval<const reverse_iterator<IRhs> &>().base() - _tt::declval<const reverse_iterator<ILhs> &>().base())
    {
        return rhs.base() - lhs.base();
    }

    template<typename ILhs, typename IRhs>
    constexpr bool operator == (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<ILhs> &>().base() ) &&
                 noexcept( _tt::declval<const reverse_iterator<IRhs> &>().base() ) &&
                 noexcept( _tt::declval<const reverse_iterator<ILhs> &>().base() == _tt::declval<const reverse_iterator<IRhs> &>().base() ) &&
                 noexcept( static_cast<bool>(_tt::declval<decltype(_tt::declval<const reverse_iterator<ILhs> &>().base() == _tt::declval<const reverse_iterator<IRhs> &>().base())>()) ))
    {
        return lhs.base() == rhs.base();
    }

    template<typename ILhs, typename IRhs>
    constexpr bool operator != (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<ILhs> &>() == _tt::declval<const reverse_iterator<IRhs> &>() ))
    {
        return !(lhs == rhs);
    }

    template<typename ILhs, typename IRhs>
    constexpr bool operator < (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<IRhs> &>().base() ) &&
                 noexcept( _tt::declval<const reverse_iterator<ILhs> &>().base() ) &&
                 noexcept( _tt::declval<const reverse_iterator<IRhs> &>().base() < _tt::declval<const reverse_iterator<ILhs> &>().base() ) &&
                 noexcept( static_cast<bool>(_tt::declval<decltype(_tt::declval<const reverse_iterator<IRhs> &>().base() < _tt::declval<const reverse_iterator<ILhs> &>().base())>()) ))
    {
        return rhs.base() < lhs.base();
    }

    template<typename ILhs, typename IRhs>
    constexpr bool operator > (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<IRhs> &>() < _tt::declval<const reverse_iterator<ILhs> &>() ))
    {
        return rhs < lhs;
    }

    template<typename ILhs, typename IRhs>
    constexpr bool operator <= (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<IRhs> &>() < _tt::declval<const reverse_iterator<ILhs> &>() ))
    {
        return !(rhs < lhs);
    }

    template<typename ILhs, typename IRhs>
    constexpr bool operator >= (const reverse_iterator<ILhs> & lhs, const reverse_iterator<IRhs> & rhs)
        noexcept(noexcept( _tt::declval<const reverse_iterator<ILhs> &>() < _tt::declval<const reverse_iterator<IRhs> &>() ))
    {
        return !(lhs < rhs);
    }

    template<class I>
    constexpr reverse_iterator<I> make_reverse_iterator(I it)
    {
        return reverse_iterator<I>(::gtfo::move(it));
    }
}

#endif // GTFO_FILE_INCLUDED_ITERATOR_REVERSE_ITERATOR_HPP
