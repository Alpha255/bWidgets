#pragma once

#include <list>
#include <assert.h>

#include "bwContainerWidget.h"
#include "screen/bwLayoutInterface.h"
#include "bwWidget.h"

namespace bWidgets
{
namespace bwScreenGraph
{

class bwEventHandler;

/**
 * \brief The base data-structure for a screen-graph node
 *
 * Screen-graph nodes are the atomical components of a screen-graph, which is
 * key for the bWidgets design. If you're not familiar with our screen-graph
 * concept, you should really have a look at the
 * [bWidgets design overview](md_docs_bWidgets_design_overview.html).
 *
 * There are three kinds of screen-graph elements/nodes:
 * * Widget nodes (node representing a simple widget)
 * * Layout nodes (node with children aligned in specified layout)
 * * Container nodes (combination of both, widget with children aligned in
 *   layout - i.e. panel)
 *
 * To ensure separation of concerns, the screen-graph node must not have any
 * functionality of its own (no non-trivial member functions). Any functionality
 * must be implemented using friend classes and functions. For example to build
 * a screen-graph and the necessary nodes, a screen-graph builder class should
 * be used and be declared as friend of the `Node` class.
 * Having to declare those helpers as friends may turn out to an annoyance
 * with small benefits. In that case we should just make data public.
 */
class bwNode
{
    friend class bwBuilder;

public:
    using ChildList = std::list<std::unique_ptr<bwNode>>;
    using ChildIterator = ChildList::iterator;

    bwNode() = default;
    virtual ~bwNode() = default;

    virtual auto Children() const -> const ChildList*
    {
        return nullptr;
    }
    virtual auto Children() -> ChildList*
    {
        return nullptr;
    }

    virtual auto childrenVisible() const -> bool
    {
        return true;
    }

    virtual auto Layout() const -> bwLayoutInterface*
    {
        return nullptr;
    }

    virtual auto Widget() const -> bwWidget*
    {
        return nullptr;
    }

    auto Parent() const -> bwNode*
    {
        return parent;
    }

    auto eventHandler() const -> bwEventHandler*
    {
        return handler.get();
    }

    virtual auto Rectangle() const -> bwRectanglePixel = 0;
    virtual auto MaskRectangle() const -> std::optional<bwRectanglePixel> = 0;
    virtual auto isVisible() const -> bool = 0;

protected:
    bwNode* parent{ nullptr };
    std::unique_ptr<bwEventHandler> handler{ nullptr };
};

/**
 * \brief Node for aligning children to a specific layout.
 */
class bwLayoutNode : virtual public bwNode
{
    friend class bwBuilder;

public:
    auto Children() const -> const ChildList* override
    {
        return &children;
    }
    auto Children() -> ChildList* override
    {
        return &children;
    }

    auto Layout() const -> bwLayoutInterface* override
    {
        return layout.get();
    }

    auto Rectangle() const -> bwRectanglePixel override
    {
        assert(layout);
        return layout->getRectangle();
    }

    auto MaskRectangle() const -> std::optional<bwRectanglePixel> override
    {
        return std::nullopt;
    }

    auto isVisible() const -> bool override
    {
        return true;
    }

    void setLayout(std::unique_ptr<bwLayoutInterface> inLayout)
    {
        layout = std::move(inLayout);
    }

    template<class Layout, typename... Args>
    Layout& createLayout(Args&&... args)
    {
        static_assert(std::is_base_of<bwLayoutInterface, Layout>::value, "should be a valid layout type");
        
        assert(layout == nullptr);
        layout = std::make_unique<Layout>(std::forward<Args>(args)...);
        return static_cast<Layout&>(*layout);
    }

private:
    std::unique_ptr<bwLayoutInterface> layout;
    ChildList children;
};

/**
 * \brief Node representing a single widget with no children.
 */
class bwWidgetNode : virtual public bwNode
{
    friend class bwBuilder;

public:
    auto Widget() const -> bwWidget* override
    {
        assert(widget);
        return &*widget;
    }

    auto Rectangle() const -> bwRectanglePixel override
    {
        assert(widget);
        return widget->rectangle;
    }

    auto MaskRectangle() const -> std::optional<bwRectanglePixel> override
    {
        return std::nullopt;
    }

    auto isVisible() const -> bool override
    {
        assert(widget);
        return widget->isHidden() == false;
    }

    void setWidget(std::unique_ptr<bwWidget> inWidget)
    {
        widget = std::move(inWidget);
        handler = widget->createHandler();
    }

     template<class Widget, typename... Args> 
     Widget& createWidget(Args&&... args)
     {
        static_assert(std::is_base_of<bwWidget, Widget>::value, "should be a valid widget type");

        assert(widget == nullptr);
        widget = std::make_unique<Widget>(std::forward<Args>(args)...);
        handler = widget->createHandler();
        return static_cast<Widget&>(*widget);
     }
private:
    std::unique_ptr<bwWidget> widget;
};

/**
 * \brief Node representing a widget with children.
 *
 * Note virtual inheritance of LayoutNode and WidgetNode, required to solve
 * diamond problems.
 */
class bwContainerNode : public bwLayoutNode, public bwWidgetNode
{
public:
    auto Children() const -> const ChildList* override
    {
        return bwLayoutNode::Children();
    }
    auto Children() -> ChildList* override
    {
        return bwLayoutNode::Children();
    }

    auto Layout() const -> bwLayoutInterface* override
    {
        return bwLayoutNode::Layout();
    }

    auto Widget() const -> bwWidget* override
    {
        return bwWidgetNode::Widget();
    }

    auto ContainerWidget() const -> bwContainerWidget&
    {
        return static_cast<bwContainerWidget&>(*Widget());
    }

    auto Rectangle() const -> bwRectanglePixel override
    {
        return bwWidgetNode::Rectangle();
    }
    auto ContentRectangle() const -> bwRectanglePixel
    {
        return bwLayoutNode::Rectangle();
    }

    auto MaskRectangle() const -> std::optional<bwRectanglePixel> override
    {
        return ContainerWidget().getMaskRectangle();
    }

    auto isVisible() const -> bool override
    {
        return bwWidgetNode::isVisible();
    }

    auto childrenVisible() const -> bool override
    {
        return ContainerWidget().childrenVisible();
    }
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
