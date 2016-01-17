#ifndef GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP
#define GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP

#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
#include "gtfo/_impl/type_traits/is_range_with_same_begin_end.hpp"
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{

#define GTFO_RANGE_REVERSE_ITERATOR \
    ::std::reverse_iterator                \
    <                                             \
        typename _tt::iterator_of_range< Range >::type \
    >

    template<typename Range>
    inline
    GTFO_RANGE_REVERSE_ITERATOR
    rbegin(Range && range)
    {
        return GTFO_RANGE_REVERSE_ITERATOR( end(_utils::forward<Range>(range)) );
    }

    template<typename Range>
    inline
    GTFO_RANGE_REVERSE_ITERATOR
    rend(Range && range)
    {
        return GTFO_RANGE_REVERSE_ITERATOR( begin(_utils::forward<Range>(range)) );
    }

    namespace detail
    {
        template<typename Range>
        class reversing_raw_pointer_to_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
        public:
            explicit reversing_raw_pointer_to_range(_range_type & range) : _ptr(_utils::addressof(range)) { }
            GTFO_RANGE_REVERSE_ITERATOR begin() const { return ::gtfo::rbegin(*_ptr); }
            GTFO_RANGE_REVERSE_ITERATOR end()   const { return ::gtfo::rend(*_ptr);   }
        private:
            _range_type * _ptr;
        };

        template<typename Range>
        class reversing_owner_of_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
        public:
            explicit reversing_owner_of_range(_range_type && range) : _range(_utils::move(range)) { }
            GTFO_RANGE_REVERSE_ITERATOR begin() { return ::gtfo::rbegin(_range); }
            GTFO_RANGE_REVERSE_ITERATOR end()   { return ::gtfo::rend(_range);   }
        private:
            _range_type _range;
        };
    }

#undef GTFO_RANGE_REVERSE_ITERATOR

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
        return detail::reversing_owner_of_range<Range>(_utils::move(range));
    }
}

#endif // GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP
