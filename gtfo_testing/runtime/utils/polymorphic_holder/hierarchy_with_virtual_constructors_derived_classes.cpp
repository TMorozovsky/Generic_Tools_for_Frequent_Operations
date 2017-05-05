#include <string>
#include <cstdio>
#include <cstring>
#include "gtfo_testing/runtime/utils/polymorphic_holder/hierarchy_with_virtual_constructors.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

class SquareWidget : public Widget
{
private:
    int _x, _y, _size;

public:
    SquareWidget(int x, int y, int size)
        : _x(x), _y(y), _size(size)
    {
    }

    void draw() const override
    {
        std::printf("SquareWidget [%d * %d] at (%d, %d)\n", _size, _size, _x, _y);
    }

    GTFO_POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE(SquareWidget)
};

class RectangleWidget : public Widget
{
private:
    int _x, _y, _w, _h;

public:
    RectangleWidget(int x, int y, int w, int h)
        : _x(x), _y(y), _w(w), _h(h)
    {
    }

    void draw() const override
    {
        std::printf("RectangleWidget [%d * %d] at (%d, %d)\n", _w, _h, _x, _y);
    }

    GTFO_POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE(RectangleWidget)
};

class AnotherBaseWhichMakesOffsetNotNull
{
public:
    virtual ~AnotherBaseWhichMakesOffsetNotNull() { }
};

class WeirdWidget : public AnotherBaseWhichMakesOffsetNotNull, public Widget
{
private:
    std::string _info;

public:
    explicit WeirdWidget(const char * info)
        : _info(info)
    {
        auto p_first_base = static_cast<AnotherBaseWhichMakesOffsetNotNull *>(this);
        GTFO_TEST_ASSERT(uintptr_t(p_first_base) == uintptr_t(this));
        (void)p_first_base;

        auto p_Widget_base = static_cast<Widget *>(this);
        GTFO_TEST_ASSERT(uintptr_t(p_Widget_base) > uintptr_t(this));
        (void)p_Widget_base;
    }

    void draw() const override
    {
        std::printf("WeirdWidget (%.12s)\n", _info.c_str());
    }

    GTFO_POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE(WeirdWidget)
};

void make_specific_widget(const char * keyword, widget_holder & dest)
{
    GTFO_TEST_ASSERT(keyword != nullptr);
    if (std::strcmp(keyword, "SquareWidget") == 0) {
        dest.reset<SquareWidget>(5, 7, 20);
    } else if (std::strcmp(keyword, "RectangleWidget") == 0) {
        dest.reset<RectangleWidget>(5, 7, 25, 35);
    } else if (std::strcmp(keyword, "WeirdWidget") == 0) {
        dest.reset<WeirdWidget>("Hello world! Also, I wanted to make this string a little bit longer so that Small String Optimization won't be applied to it when it is stored inside a std::string =^__^=");
    } else {
        GTFO_TEST_ASSERT(!"Unknown keyword!");
        dest.reset();
    }
}

namespace {
    struct print_sizes_of_derived_classes {
        print_sizes_of_derived_classes() {
            std::printf("sizeof(%s) is %zu\n", "SquareWidget",    sizeof(SquareWidget));
            std::printf("sizeof(%s) is %zu\n", "RectangleWidget", sizeof(RectangleWidget));
            std::printf("sizeof(%s) is %zu\n", "WeirdWidget",     sizeof(WeirdWidget));
            std::printf("\n");
            std::printf("sizeof(%s) is %zu\n", "widget_holder",   sizeof(widget_holder));
            std::printf("\n");

            using tt_result = gtfo::polymorphic_holder_utils::type_traits<SquareWidget, RectangleWidget, WeirdWidget>;
            std::printf("max size of these types = %zu\n",        tt_result::max_size);
            std::printf("max alignment of these types = %zu\n",   tt_result::max_alignment);
        }
    };// print_sizes_of_derived_classes_instance;
}