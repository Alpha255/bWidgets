/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#pragma once

#include "bwRectangle.h"

#include "screen_graph/bwNode.h"

namespace bWidgets
{
class bwPoint;
class bwStyle;
class bwWidget;
}  // namespace bWidgets

namespace bWidgets
{

void resolveScreenGraphNodeLayout(bWidgets::bwScreenGraph::bwLayoutNode& node,
                                  const bWidgets::bwRectangle<float>& rect,
                                  const float scale_fac);

/**
 * \brief An abstract class for defining items that form the layout.
 *
 * The bwLayoutItem class provides needed functionality to define and draw widgets,
 * which get automatically placed and sized based on the layout-item type.
 * Each item can have its own sub-items with different types.
 *
 * To update position and sizes of an item and it's sub-items, call resolve(). Call
 * draw() to draw the widgets based on positions and sizes resolve() calculated.
 * Normally, draw() is called immediately after resolve().
 *
 * \note This currently is rather basic and meant for testing bWidgets mainly. It's
 *       planned to give bWidgets its own layout-engine at some point.
 * \note We currently expect a RootLayout to be at the top of the tree. Through it
 *       the needed position and size hints can be set. Without changing these, the
 *       calculated widget-coordinates don't change.
 */
class bwLayoutItem : public bWidgets::bwLayoutInterface
{
public:
    enum class Type
    {
        ROW,
        COLUMN,
        PANEL,
        SCROLL_VIEW
    };

    enum FlowDirection
    {
        // Widgets and child-layouts are added top to down
        FLOW_DIRECTION_VERTICAL,
        // Widgets and child-layouts are added left to right
        FLOW_DIRECTION_HORIZONTAL,
    };

    virtual ~bwLayoutItem() override = default;

    virtual void resolve(bWidgets::bwScreenGraph::bwNode& node,
                         const bWidgets::bwPoint& layout_pos,
                         const unsigned int item_margin,
                         const float scale_fac);

    auto getRectangle() -> bWidgets::bwRectanglePixel override;
    auto getHeight() const -> unsigned int;

    inline bwLayoutItem& setPadding(uint32_t inPadding)
    {
        padding = inPadding;
        return *this;
    }

    const Type type;
    const FlowDirection flow_direction;
    unsigned int padding = 0;
    const bool align;

protected:
    using LayoutItemList = std::list<std::unique_ptr<bwLayoutItem>>;
    using IteratorItem = LayoutItemList::const_iterator;

    // Protected constructor to force calling through inherited class (pseudo abstract).
    bwLayoutItem(Type layout_type,
                 const bool align,
                 FlowDirection flow_direction = FLOW_DIRECTION_HORIZONTAL);

    static void resolvePanelContents(bWidgets::bwScreenGraph::bwNode& panel_node,
                                     const bWidgets::bwPoint& panel_pos,
                                     const unsigned int padding,
                                     const unsigned int item_margin,
                                     const float scale_fac);

    int width{ 0 }, height{ 0 };
    bWidgets::bwPoint location;

private:
    auto countRowColumns(const bWidgets::bwScreenGraph::bwNode::ChildList& children) const
        -> unsigned int;
    auto countNeededMargins(const bWidgets::bwScreenGraph::bwNode::ChildList& children) const
        -> unsigned int;
};

class bwColumnLayout : public bwLayoutItem
{
public:
    explicit bwColumnLayout(const bool align = false);
};

class bwRowLayout : public bwLayoutItem
{
public:
    explicit bwRowLayout(const bool align = false);
};

class bwPanelLayout : public bwLayoutItem
{
public:
    explicit bwPanelLayout();
};

class bwScrollViewLayout : public bwLayoutItem
{
public:
    explicit bwScrollViewLayout();

    void resolve(bWidgets::bwScreenGraph::bwNode& node,
                 const bWidgets::bwPoint& layout_pos,
                 const unsigned int item_margin,
                 const float scale_fac) override;

    inline bwScrollViewLayout& setItemMargin(uint32_t margin)
    {
        item_margin = margin;
        return *this;
    }

    unsigned int item_margin = 0;
};

}  // namespace bWidgets
