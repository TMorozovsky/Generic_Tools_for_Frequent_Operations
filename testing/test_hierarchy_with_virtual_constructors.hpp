#ifndef TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
#define TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED

#include "polymorphic_holder.hpp"

class Widget : public polymorphic_holder_utils::nothrow_moveable_and_throwing_copyable
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

using widget_holder = polymorphic_holder<Widget, MAX_SIZEOF_DERIVED_WIDGET, 8,
                                         polymorphic_holder_MCP::nothrow_moveable_and_throwing_copyable,
                                         polymorphic_holder_DRP::offset_after_bytes_typed<unsigned int>>;

extern void make_specific_widget(const char * keyword, widget_holder & dest);

#undef MAX_SIZEOF_DERIVED_WIDGET

#endif // TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
