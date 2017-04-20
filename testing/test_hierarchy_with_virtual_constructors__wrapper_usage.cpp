#include "test_hierarchy_with_virtual_constructors.hpp"
#include <cstdio>
#include <vector>
#include <utility>

class OurOwnDerivedWidget : public Widget
{
public:
    OurOwnDerivedWidget()
    {
    }

    void draw() const override
    {
        std::printf("OurOwnDerivedWidget\n");
    }

    POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE(OurOwnDerivedWidget)
};

static int test()
{
    std::vector<widget_holder> widgets;

    const char * keywords[] = { "SquareWidget", "RectangleWidget", "WeirdWidget" };
    int iter = 0;
    for (auto keyword : keywords) {
        widget_holder tmp_w;
        ::make_specific_widget(keyword, tmp_w);
        assert(tmp_w);

        // Try different ways of pushing a polymorphic_holder into a vector:
        if (iter == 0) {
            widgets.push_back(tmp_w); // copy construction
        } else if (iter == 1) {
            widgets.push_back(std::move(tmp_w)); // move construction
        } else if (iter == 2) {
            widgets.push_back(widget_holder()); // default construction + copy assignment
            widgets.back() = tmp_w;
        }

        assert(iter < 3);
        ++iter;
    }

    widgets.emplace_back();
    widgets.back() = widget_holder::make<OurOwnDerivedWidget>(); // default construction + move assignment

    auto widgets_copy = widgets; // make a copy of the whole vector
    widgets = std::move(widgets_copy); // now move it back

    std::printf("Looping through a vector of %zu objects derived from Widget:\n", widgets.size());
    for (auto & w : widgets) {
        std::putchar('\t');
        w->draw();
    }

    return 0;
}

int main()
{
    int res = test();
#ifdef _MSC_VER
    std::getchar();
#endif
    return res;
}
