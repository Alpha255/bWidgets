#pragma once

#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets
{

namespace bwScreenGraph
{
class bwContainerNode;
}

class bwContainerWidget : public bwWidget
{
public:
    virtual bwRectanglePixel getMaskRectangle();
    virtual bool childrenVisible() const;

    void registerProperties() override;

protected:
    bwContainerWidget(const bwScreenGraph::bwContainerNode& node,
                      std::optional<uint32_t> width_hint = std::nullopt,
                      std::optional<uint32_t> height_hint = std::nullopt);

    /** Reference to node owning this widget, for additional info queries. */
    const bwScreenGraph::bwContainerNode& node;

public:
    bwWidgetBaseStyle base_style;  // XXX public for setWidgetStyle. Should only
                                   // be temporarily needed.
};

}  // namespace bWidgets
