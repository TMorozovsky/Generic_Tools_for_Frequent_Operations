#ifndef GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP
#define GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP

#include "gtfo/_impl/type_traits/iterator_of_range.hpp"
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
        return GTFO_RANGE_REVERSE_ITERATOR( end(::gtfo::forward<Range>(range)) );
    }

    template<typename Range>
    inline
    GTFO_RANGE_REVERSE_ITERATOR
    rend(Range && range)
    {
        return GTFO_RANGE_REVERSE_ITERATOR( begin(::gtfo::forward<Range>(range)) );
    }


    namespace detail
    {
        template<typename Range>
        class reversing_raw_pointer_to_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
        public:
            explicit reversing_raw_pointer_to_range(_range_type & range) : _ptr(::gtfo::addressof(range)) { }
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
            explicit reversing_owner_of_range(_range_type && range) : _range(::gtfo::move(range)) { }
            GTFO_RANGE_REVERSE_ITERATOR begin() { return ::gtfo::rbegin(_range); }
            GTFO_RANGE_REVERSE_ITERATOR end()   { return ::gtfo::rend(_range);   }
        private:
            _range_type _range;
        };
    }

#undef GTFO_RANGE_REVERSE_ITERATOR

    template<typename Range>
    inline
    typename _tt::enable_if
    <
        _tt::is_lvalue_reference<Range>::value,
        detail::reversing_raw_pointer_to_range<Range>
    >::type
    rev(Range && range)
    {
        return detail::reversing_raw_pointer_to_range<Range>(range);
    }

    template<typename Range>
    inline
    typename _tt::enable_if
    <
        !_tt::is_lvalue_reference<Range>::value,
        detail::reversing_owner_of_range<Range>
    >::type
    rev(Range && range)
    {
        return detail::reversing_owner_of_range<Range>(::gtfo::move(range));
    }
}

#endif // GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP

