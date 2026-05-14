#include <cassert>
#include <cmath>

#include "event/bwEvent.h"
#include "paint/bwPainter.h"
#include "bwRange.h"
#include "bwStyle.h"

#include "screen_graph/bwBuilder.h"
#include "screen_graph/bwDrawer.h"
#include "screen_graph/bwNode.h"

#include "bwScrollBar.h"

#include "bwScrollView.h"

namespace bWidgets
{

bwScrollView::bwScrollView(bwScreenGraph::bwContainerNode& node,
                           uint32_t _width,
                           uint32_t _height)
    : bwContainerWidget(node, _width, _height)
{
    auto scrollbar = std::make_unique<bwScrollBar>(17, _height);
    scrollbar_node = std::make_unique<bwScreenGraph::bwWidgetNode>();

    bwScreenGraph::bwBuilder::setWidget(*scrollbar_node, std::move(scrollbar));
}

std::string_view bwScrollView::getTypeIdentifier() const
{
    return "bwScrollView";
}

bwScrollBar& bwScrollView::getVerticalScrollBar() const
{
    return static_cast<bwScrollBar&>(*scrollbar_node->Widget());
}

bwRectanglePixel bwScrollView::getVerticalScrollbarRect(const bwStyle& style) const
{
    bwRectanglePixel scroll_rectangle{ rectangle };
    /* TODO hardcoded padding */
    const int32_t padding = 4 * (int32_t)style.dpi_fac;

    scroll_rectangle.xmin = scroll_rectangle.xmax -
                            bwScrollView::getScrollbarWidth(style.dpi_fac) - padding;
    scroll_rectangle.resize(-padding);

    return scroll_rectangle;
}

void bwScrollView::drawScrollBars(bwStyle& style)
{
    bwScrollBar& scrollbar = getVerticalScrollBar();

    validizeScrollValues();

    scrollbar.rectangle = getVerticalScrollbarRect(style);
    scrollbar.ratio = (rectangle.height() - 2) / float(node.ContentRectangle().height());
    scrollbar.scroll_offset = vert_scroll;

    bwScreenGraph::bwDrawer::drawSubtree(*scrollbar_node, style);
}

void bwScrollView::draw(bwStyle& style)
{
    bwPainter painter;

    painter.active_drawtype = bwPainter::DrawType::FILLED;
    painter.setActiveColor(base_style.background_color);
    painter.drawRectangle(rectangle);

    if (base_style.isBorderVisible())
    {
        painter.active_drawtype = bwPainter::DrawType::OUTLINE;
        painter.setActiveColor(base_style.border_color);
        painter.drawRectangle(rectangle);
    }
    if (isScrollable())
    {
        drawScrollBars(style);
    }
}

void bwScrollView::validizeScrollValues()
{
    assert(isScrollable());

    bwRange<int32_t>::clampValue(vert_scroll, 0, node.ContentRectangle().height() - node.Rectangle().height());
}

int32_t bwScrollView::getScrollOffsetY() const
{
    return vert_scroll;
}

bwRectanglePixel bwScrollView::getContentBounds(float interface_scale) const
{
    bwRectanglePixel bounds{ rectangle };
    if (isScrollable())
    {
        bounds.xmax -= getScrollbarWidth(interface_scale);
    }
    return bounds;
}

bool bwScrollView::isScrollable() const
{
    return (node.ContentRectangle().height() > node.Rectangle().height()) || (vert_scroll != 0);
}

int32_t bwScrollView::getScrollbarWidth(float interface_scale)
{
    return std::round(SCROLL_BAR_SIZE * interface_scale);
}

// ------------------ Handling ------------------

class bwScrollViewHandler : public bwScreenGraph::bwEventHandler
{
public:
    bwScrollViewHandler(bwScrollView& scrollview);
    ~bwScrollViewHandler() = default;

    void onMouseMove(bwEvent&) override;
    void onMouseEnter(bwEvent&) override;
    void onMouseLeave(bwEvent&) override;
    void onMousePress(bwMouseButtonEvent& event) override;
    void onMouseRelease(bwMouseButtonEvent& event) override;
    void onMouseClick(bwMouseButtonEvent& event) override;
    void onMouseDrag(bwMouseButtonDragEvent& event) override;
    void onMouseWheel(bwMouseWheelEvent& event) override;

    void onScrollbarMouseEnter(bwEvent& event) const;
    void onScrollbarMouseLeave(bwEvent& event) const;

    auto isEventInsideScrollbar(const class bwEvent& event) const -> bool;

    void setScrollValue(int32_t value);

private:
    bwScrollView& scrollview;
    constexpr static int32_t SCROLL_STEP_SIZE = 40;

    bool was_inside_scrollbar{ false };
};

bwScrollViewHandler::bwScrollViewHandler(bwScrollView& panel) : scrollview(panel)
{
}

std::unique_ptr<bwScreenGraph::bwEventHandler> bwScrollView::createHandler()
{
    return std::make_unique<bwScrollViewHandler>(*this);
}

template<typename... _Args>
using HandlerFunc = void (bwScreenGraph::bwEventHandler::*)(_Args&&...);

/* Could turn this into a general utility to forward events to different
 * widgets. */
template<typename... _Args>
static void forwardEventToNode(bwScreenGraph::bwNode& to_node,
                               HandlerFunc<_Args&&...> handler_func,
                               _Args&&... __args)
{
    (to_node.eventHandler()->*handler_func)(std::forward<_Args>(__args)...);
}

template<typename... _Args>
static auto forwardEventToScrollbarIfInside(const bwScrollViewHandler& scrollview_handler,
                                            bwScreenGraph::bwNode& scrollbar_node,
                                            const class bwEvent& event,
                                            HandlerFunc<_Args&&...> handler_func,
                                            _Args&&... __args)
{
    if (scrollview_handler.isEventInsideScrollbar(event))
    {
        forwardEventToNode<_Args&&...>(
            scrollbar_node, handler_func, std::forward<_Args>(__args)...);
        return true;
    }

    return false;
}

void bwScrollViewHandler::onMouseWheel(bwMouseWheelEvent& event)
{
    if (!scrollview.isScrollable())
    {
        return;
    }

    char direction_fac = 0;

    switch (event.getDirection())
    {
    case bwMouseWheelEvent::Direction::UP:
        direction_fac = -1;
        break;
    case bwMouseWheelEvent::Direction::DOWN:
        direction_fac = 1;
        break;
    }

    setScrollValue(scrollview.vert_scroll + (direction_fac * SCROLL_STEP_SIZE));

    event.swallow();
}

bool bwScrollViewHandler::isEventInsideScrollbar(const bwEvent& event) const
{
    return scrollview.isScrollable() && scrollview.scrollbar_node->Rectangle().isCoordinateInside(
                                            event.location.x, event.location.y);
}

void bwScrollViewHandler::onScrollbarMouseEnter(bwEvent& event) const
{
    forwardEventToNode<bwEvent&>(*scrollview.scrollbar_node, &bwScreenGraph::bwEventHandler::onMouseEnter, event);
}

void bwScrollViewHandler::onScrollbarMouseLeave(bwEvent& event) const
{
    forwardEventToNode<bwEvent&>(*scrollview.scrollbar_node, &bwScreenGraph::bwEventHandler::onMouseLeave, event);
}

void bwScrollViewHandler::onMouseMove(bwEvent& event)
{
    forwardEventToScrollbarIfInside<bwEvent&>(
        *this, *scrollview.scrollbar_node, event, &bwScreenGraph::bwEventHandler::onMouseMove, event);

    if (was_inside_scrollbar && !isEventInsideScrollbar(event))
    {
        onScrollbarMouseLeave(event);
        was_inside_scrollbar = false;
    }
    else if (!was_inside_scrollbar && isEventInsideScrollbar(event))
    {
        onScrollbarMouseEnter(event);
        was_inside_scrollbar = true;
    }
}

void bwScrollViewHandler::onMouseEnter(bwEvent& event)
{
    if (!was_inside_scrollbar && isEventInsideScrollbar(event))
    {
        onScrollbarMouseEnter(event);
        was_inside_scrollbar = true;
    }
}

void bwScrollViewHandler::onMouseLeave(bwEvent& event)
{
    if (was_inside_scrollbar && !isEventInsideScrollbar(event))
    {
        onScrollbarMouseLeave(event);
        was_inside_scrollbar = false;
    }
}

void bwScrollViewHandler::onMouseDrag(bwMouseButtonDragEvent& event)
{
    if (forwardEventToScrollbarIfInside<bwMouseButtonDragEvent&>(
            *this, *scrollview.scrollbar_node, event, &bwScreenGraph::bwEventHandler::onMouseDrag, event))
    {
        setScrollValue(scrollview.getVerticalScrollBar().scroll_offset);
        event.swallow();
    }
}

void bwScrollViewHandler::onMousePress(bwMouseButtonEvent& event)
{
    if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
            *this, *scrollview.scrollbar_node, event, &bwScreenGraph::bwEventHandler::onMousePress, event))
    {
        event.swallow();
    }
}

void bwScrollViewHandler::onMouseRelease(bwMouseButtonEvent& event)
{
    if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
            *this, *scrollview.scrollbar_node, event, &bwScreenGraph::bwEventHandler::onMouseRelease, event))
    {
        event.swallow();
    }
}

void bwScrollViewHandler::onMouseClick(bwMouseButtonEvent& event)
{
    if (forwardEventToScrollbarIfInside<bwMouseButtonEvent&>(
            *this, *scrollview.scrollbar_node, event, &bwScreenGraph::bwEventHandler::onMouseClick, event))
    {
        setScrollValue(scrollview.getVerticalScrollBar().scroll_offset);
        event.swallow();
    }
}

void bwScrollViewHandler::setScrollValue(int32_t value)
{
    assert(scrollview.isScrollable());

    scrollview.vert_scroll = value;
    scrollview.validizeScrollValues();
}

}  // namespace bWidgets
