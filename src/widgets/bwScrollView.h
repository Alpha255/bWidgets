#pragma once

#include <cmath>

#include "bwContainerWidget.h"

namespace bWidgets
{

namespace bwScreenGraph
{
class bwWidgetNode;
}
class bwScrollBar;

class bwScrollView : public bwContainerWidget
{
    friend class bwScrollViewHandler;

public:
    bwScrollView(bwScreenGraph::bwContainerNode& node,
                 unsigned int width = 0,
                 unsigned int height = 0);

    auto getTypeIdentifier() const -> std::string_view override;

    void draw(bwStyle& style) override;

    auto createHandler() -> std::unique_ptr<bwScreenGraph::bwEventHandler> override;

    auto getScrollOffsetY() const -> int;
    auto getContentBounds(float interface_scale) const -> bwRectanglePixel;

private:
    auto getVerticalScrollBar() const -> bwScrollBar&;
    auto getVerticalScrollbarRect(const bwStyle& style) const -> bwRectanglePixel;
    void drawScrollBars(bwStyle& style);
    auto isScrollable() const -> bool;
    void validizeScrollValues();

    static auto getScrollbarWidth(float interface_scale) -> int;

    constexpr static int SCROLL_BAR_SIZE = 17;

    std::unique_ptr<bwScreenGraph::bwWidgetNode> scrollbar_node;

    int vert_scroll{ 0 };
};

}  // namespace bWidgets
