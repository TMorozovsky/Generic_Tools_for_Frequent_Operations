#ifndef GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP
#define GTFO_FILE_INCLUDED_REVERSED_RANGE_HPP

#include "gtfo/_impl/rbegin_rend.hpp"
#include "gtfo/_impl/addressof.hpp"
#include "gtfo/_impl/move.hpp"

namespace gtfo
{
    namespace detail
    {
        template<typename Range>
        class reversing_raw_pointer_to_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
            typedef decltype(rbegin(::gtfo::_tt::declval<_range_type &>())) _rbegin_type;
            typedef decltype(rend  (::gtfo::_tt::declval<_range_type &>())) _rend_type;
        public:
            explicit reversing_raw_pointer_to_range(_range_type & range) : _ptr(::gtfo::addressof(range)) { }
            _rbegin_type begin() const { return rbegin(*_ptr); }
            _rend_type   end()   const { return rend(*_ptr);   }
        private:
            _range_type * _ptr;
        };

        template<typename Range>
        class reversing_owner_of_range
        {
            typedef typename _tt::remove_reference<Range>::type _range_type;
            typedef decltype(::gtfo::rbegin(::gtfo::_tt::declval<_range_type &>())) _rbegin_type;
            typedef decltype(::gtfo::rend  (::gtfo::_tt::declval<_range_type &>())) _rend_type;
        public:
            explicit reversing_owner_of_range(_range_type && range) : _range(::gtfo::move(range)) { }
            _rbegin_type begin() { return ::gtfo::rbegin(_range); }
            _rend_type   end()   { return ::gtfo::rend(_range);   }
        private:
            _range_type _range;
        };
    }

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

