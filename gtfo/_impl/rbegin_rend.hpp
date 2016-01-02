#ifndef GTFO_FILE_INCLUDED_RBEGIN_REND_HPP
#define GTFO_FILE_INCLUDED_RBEGIN_REND_HPP

#include "gtfo/_impl/type_traits/has_rbegin_mem_fun.hpp"
#include "gtfo/_impl/type_traits/has_rend_mem_fun.hpp"

namespace gtfo
{
    namespace detail
    {
        template<typename Range, bool range_has_member_rbegin>
        struct impl_result_of_rbegin
        {
        };

        template<typename Range>
        struct impl_result_of_rbegin<Range, true>
        {
            typedef decltype( ::gtfo::_tt::declval<Range &>().rbegin() ) type;
        };

        template<typename Range>
        struct result_of_rbegin : impl_result_of_rbegin
                                  <
                                      Range,
                                      ::gtfo::_tt::has_rbegin_mem_fun<Range>::value
                                  >
        {
        };

        template<typename Range, bool range_has_member_rend>
        struct impl_result_of_rend
        {
        };

        template<typename Range>
        struct impl_result_of_rend<Range, true>
        {
            typedef decltype( ::gtfo::_tt::declval<Range &>().rend() ) type;
        };

        template<typename Range>
        struct result_of_rend : impl_result_of_rend
                                <
                                    Range,
                                    ::gtfo::_tt::has_rend_mem_fun<Range>::value
                                >
        {
        };

        template<typename ArrayElement>
        class array_reverse_iterator : public ::std::iterator< ::std::random_access_iterator_tag, ArrayElement >
        {
        public:
            array_reverse_iterator() // _ptr is left uninitialized
            {
            }

            friend bool operator == (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return lhs._ptr == rhs._ptr;
            }

            friend bool operator != (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return lhs._ptr != rhs._ptr;
            }

            ArrayElement & operator * () const
            {
                // TODO : maybe add some debugging checks that can be turned on by some preprocessor definition
                return *_ptr;
            }

            ArrayElement * operator -> () const
            {
                // TODO : same thing (see above)
                return _ptr;
            }

            array_reverse_iterator & operator ++ ()
            {
                --_ptr;
                return *this;
            }

            array_reverse_iterator operator ++ (int)
            {
                array_reverse_iterator old(*this);
                --_ptr;
                return old;
            }

            array_reverse_iterator & operator -- ()
            {
                ++_ptr;
                return *this;
            }

            array_reverse_iterator operator -- (int)
            {
                array_reverse_iterator old(*this);
                ++_ptr;
                return old;
            }

            array_reverse_iterator & operator += (ptrdiff_t n)
            {
                _ptr -= n;
                return *this;
            }

            array_reverse_iterator & operator -= (ptrdiff_t n)
            {
                _ptr += n;
                return *this;
            }

            friend array_reverse_iterator operator + (array_reverse_iterator lhs, ptrdiff_t n)
            {
                lhs._ptr -= n;
                return lhs;
            }

            friend array_reverse_iterator operator + (ptrdiff_t n, array_reverse_iterator rhs)
            {
                rhs._ptr -= n;
                return rhs;
            }

            friend array_reverse_iterator operator - (array_reverse_iterator lhs, ptrdiff_t n)
            {
                lhs._ptr += n;
                return lhs;
            }

            friend ptrdiff_t operator - (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return rhs._ptr - lhs._ptr;
            }

            friend bool operator < (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return lhs._ptr > rhs._ptr;
            }

            friend bool operator > (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return lhs._ptr < rhs._ptr;
            }

            friend bool operator <= (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return lhs._ptr >= rhs._ptr;
            }

            friend bool operator >= (array_reverse_iterator lhs, array_reverse_iterator rhs)
            {
                return lhs._ptr <= rhs._ptr;
            }

            ArrayElement & operator [] (ptrdiff_t n)
            {
                return *(_ptr - n);
            }

            template<size_t ArraySize>
            static array_reverse_iterator make_rbegin( ArrayElement(&arg_array)[ArraySize] )
            {
                return array_reverse_iterator(static_cast<ArrayElement *>(arg_array) + (ptrdiff_t(ArraySize) - 1));
            }

            template<size_t ArraySize>
            static array_reverse_iterator make_rend( ArrayElement(&arg_array)[ArraySize] )
            {
                return array_reverse_iterator(static_cast<ArrayElement *>(arg_array) - 1);
            }

        private:
            explicit array_reverse_iterator(ArrayElement * ptr)
                : _ptr(ptr)
            {
            }

        private:
            ArrayElement * _ptr;
        };
    }

    template<typename Range>
    inline
    typename detail::result_of_rbegin<Range>::type
    rbegin(Range && range)
    {
        return range.rbegin();
    }

    template<typename Range>
    inline
    typename detail::result_of_rend<Range>::type
    rend(Range && range)
    {
        return range.rend();
    }

    template<typename ArrayElement, size_t ArraySize>
    inline
    detail::array_reverse_iterator<ArrayElement>
    rbegin( ArrayElement(&range)[ArraySize] )
    {
        return detail::array_reverse_iterator<ArrayElement>::make_rbegin(range);
    }

    template<typename ArrayElement, size_t ArraySize>
    inline
    detail::array_reverse_iterator<ArrayElement>
    rend( ArrayElement(&range)[ArraySize] )
    {
        return detail::array_reverse_iterator<ArrayElement>::make_rend(range);
    }
}

#endif // GTFO_FILE_INCLUDED_RBEGIN_REND_HPP
