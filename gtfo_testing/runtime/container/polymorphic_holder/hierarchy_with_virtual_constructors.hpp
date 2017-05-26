#ifndef TESTING_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
#define TESTING_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED

#include "gtfo/container/polymorphic_holder.hpp"

class Widget : public gtfo::polymorphic_holder_utils::nothrow_moveable_and_throwing_copyable
{
public:
    virtual void draw() const = 0;
    virtual ~Widget() { }
};

#ifndef NDEBUG
#   define MAX_SIZEOF_DERIVED_WIDGET (64)
#else
#   define MAX_SIZEOF_DERIVED_WIDGET (56)
#endif

using widget_holder = gtfo::polymorphic_holder<Widget, MAX_SIZEOF_DERIVED_WIDGET, 8,
                                               gtfo::polymorphic_holder_MCP::nothrow_moveable_and_throwing_copyable,
                                               gtfo::polymorphic_holder_DRP::offset_after_bytes_typed<unsigned int>>;

extern void make_specific_widget(const char * keyword, widget_holder & dest);

#undef MAX_SIZEOF_DERIVED_WIDGET

#endif // TESTING_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
