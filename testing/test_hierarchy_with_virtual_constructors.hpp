#pragma once

#ifndef TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
#define TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED

#include "polymorphic_holder.hpp"

class Widget : public polymorphic_holder_utils::copyable_and_moveable
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
                                         polymorphic_holder_MCP::virtual_constructors,
                                         polymorphic_holder_DRP::offset_after_bytes_typed<unsigned int>>;

extern void make_specific_widget(const char * keyword, widget_holder & dest);

#undef MAX_SIZEOF_DERIVED_WIDGET

#endif // TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
