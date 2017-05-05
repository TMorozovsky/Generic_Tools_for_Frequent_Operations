#ifndef GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP
#define GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP

#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/is_range_with_same_begin_end.hpp"
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    template<typename Range>
    inline
    ::std::reverse_iterator<typename _tt::iterator_of_range<Range>::type>
    rbegin(Range && range)
    {
        return ::std::reverse_iterator<typename _tt::iterator_of_range<Range>::type>( end(::gtfo::forward<Range>(range)) );
    }

    template<typename Range>
    inline
    ::std::reverse_iterator<typename _tt::iterator_of_range<Range>::type>
    rend(Range && range)
    {
        return ::std::reverse_iterator<typename _tt::iterator_of_range<Range>::type>( begin(::gtfo::forward<Range>(range)) );
    }

    namespace detail
    {
        template<typename Range>
        class reversing_raw_pointer_to_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
            typedef typename _tt::iterator_of_range<Range>::type _range_iterator_type;
        public:
            explicit reversing_raw_pointer_to_range(_range_type & range) : _ptr(::gtfo::addressof(range)) { }
            ::std::reverse_iterator<_range_iterator_type> begin() const { return ::gtfo::rbegin(*_ptr); }
            ::std::reverse_iterator<_range_iterator_type> end()   const { return ::gtfo::rend(*_ptr);   }
        private:
            _range_type * _ptr;
        };

        template<typename Range>
        class reversing_owner_of_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
            typedef typename _tt::iterator_of_range<Range>::type _range_iterator_type;
        public:
            explicit reversing_owner_of_range(_range_type && range) : _range(::gtfo::move(range)) { }
            ::std::reverse_iterator<_range_iterator_type> begin() { return ::gtfo::rbegin(_range); }
            ::std::reverse_iterator<_range_iterator_type> end()   { return ::gtfo::rend(_range);   }
        private:
            _range_type _range;
        };
    }

    /// function template that returns a view object
    /// through which a range can be accessed in reverse order;
    /// this overload works with lvalue ranges only
    /// and its return value acts like a raw pointer to given range,
    /// so keep that in mind if you want to save this function's result
    /// for using it later (which is not recommended in general case)
    template<typename Range>
    inline
    typename _tt::enable_if
    <
        _tt::is_lvalue_reference<Range>::value && _tt::is_range_with_same_begin_end<Range>::value,
        detail::reversing_raw_pointer_to_range<Range>
    >::type
    rev(Range && range)
    {
        return detail::reversing_raw_pointer_to_range<Range>(range);
    }

    /// function template that returns a new range owner object
    /// through which a range can be accessed in reverse order;
    /// this overload works with rvalue ranges only
    /// and its return value stores the entire range in its internal state
    template<typename Range>
    inline
    typename _tt::enable_if
    <
        !_tt::is_lvalue_reference<Range>::value && _tt::is_range_with_same_begin_end<Range>::value,
        detail::reversing_owner_of_range<Range>
    >::type
    rev(Range && range)
    {
        return detail::reversing_owner_of_range<Range>(::gtfo::move(range));
    }
}

#endif // GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP
