#pragma once

#ifndef TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
#define TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED

#include "polymorphic_wrapper.hpp"

class Widget : public polymorphic_wrapper_copyable
{
public:
    virtual void draw() const = 0;
    virtual ~Widget() { }
};

using widget_wrapper = polymorphic_wrapper<Widget, 56, 8, polymorphic_wrapper_MCP::virtual_constructors>;

extern void make_specific_widget(const char * keyword, widget_wrapper & dest);

#endif // TEST_HIERARCHY_WITH_VIRTUAL_CONSTRUCTORS_HPP_INCLUDED
