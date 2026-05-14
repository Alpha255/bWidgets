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

#include <cassert>
#include <iostream>

#include "builtin_widgets.h"
#include "paint/bwPainter.h"

#include "screen/bwLayout.h"

namespace bWidgets
{

void resolveScreenGraphNodeLayout(bwScreenGraph::bwLayoutNode& node,
                                  const bwRectangle<float>& rect,
                                  const float scale_fac)
{
    if (bwLayoutItem* layout = static_cast<bwLayoutItem*>(node.Layout()))
    {
        if (bwScrollViewLayout* root = dynamic_cast<bwScrollViewLayout*>(layout))
        {
            assert(node.Children());

            if (bwWidget* widget = node.Widget())
            {
                widget->width_hint = rect.width();
                widget->height_hint = rect.height();
            }

            root->resolve(node, { rect.xmin, rect.ymin }, root->item_margin, scale_fac);
        }
    }
}

bwLayoutItem::bwLayoutItem(bwLayoutItem::Type item_type, const bool align, FlowDirection flow_direction)
    : type(item_type), flow_direction(flow_direction), align(align)
{
}

uint32_t bwLayoutItem::getHeight() const
{
    return height;
}

static bwScreenGraph::bwNode* getNextUnhiddenNode(const bwScreenGraph::bwNode::ChildList::const_iterator& current)
{
    for (auto iter = ++bwScreenGraph::bwNode::ChildList::const_iterator(current);
         iter != (*current)->Parent()->Children()->end();
         ++iter)
    {
        if ((*iter)->isVisible())
        {
            return iter->get();
        }
    }

    return nullptr;
}
static bwScreenGraph::bwNode* getPreviousUnhiddenNode(const bwScreenGraph::bwNode::ChildIterator& current)
{
    for (auto iter = bwScreenGraph::bwNode::ChildList::reverse_iterator(current);
         iter != (*current)->Parent()->Children()->rend();
         ++iter)
    {
        if ((*iter)->isVisible())
        {
            return iter->get();
        }
    }

    return nullptr;
}

static bool shouldWidgetAlignToPrevious(const bwScreenGraph::bwNode::ChildIterator& current)
{
    bwScreenGraph::bwNode* prev_item = getPreviousUnhiddenNode(current);
    bwWidget* prev_widget = prev_item ? prev_item->Widget() : nullptr;

    if (!prev_item || !prev_widget || !prev_widget->canAlign())
    {
        return false;
    }

    return true;
}
static bool shouldWidgetAlignToNext(const bwScreenGraph::bwNode::ChildIterator& current)
{
    bwScreenGraph::bwNode* next_item = getNextUnhiddenNode(current);
    bwWidget* next_widget = next_item ? next_item->Widget() : nullptr;

    if (!next_item || !next_widget || !next_widget->canAlign())
    {
        return false;
    }

    return true;
}
static void alignNode(bwScreenGraph::bwNode::ChildIterator node_iter,
                      const bwLayoutItem::FlowDirection flow_direction)
{
    bwWidget* widget = (*node_iter)->Widget();
    bwAbstractButton* abstract_button;

    if (!widget || !widget->canAlign() ||
        !(abstract_button = widget_cast<bwAbstractButton>(widget)))
    {
        return;
    }
    abstract_button->rounded_corners = 0;

    if (!shouldWidgetAlignToPrevious(node_iter))
    {
        abstract_button->rounded_corners |= (flow_direction ==
                                             bwLayoutItem::FLOW_DIRECTION_HORIZONTAL) ?
                                                (TOP_LEFT | BOTTOM_LEFT) :
                                                (TOP_LEFT | TOP_RIGHT);
    }
    if (!shouldWidgetAlignToNext(node_iter))
    {
        abstract_button->rounded_corners |= (flow_direction ==
                                             bwLayoutItem::FLOW_DIRECTION_HORIZONTAL) ?
                                                (TOP_RIGHT | BOTTOM_RIGHT) :
                                                (BOTTOM_LEFT | BOTTOM_RIGHT);
    }
}

static bool needsMarginAfterNode(const bwScreenGraph::bwNode::ChildList::const_iterator node_iter,
                                 const bool align)
{
    const bwWidget* widget = (*node_iter)->Widget();
    const bwScreenGraph::bwNode* next_iter = getNextUnhiddenNode(node_iter);
    const bwWidget* next_widget = next_iter ? next_iter->Widget() : nullptr;

    if (!next_iter)
    {
        return false;
    }

    if (align && widget && next_widget)
    {
        if (widget->canAlign() && next_widget->canAlign())
        {
            return false;
        }
    }

    return true;
}

void bwLayoutItem::resolvePanelContents(bwScreenGraph::bwNode& panel_node,
                                        const bwPoint& panel_pos,
                                        const uint32_t padding,
                                        const uint32_t item_margin,
                                        const float scale_fac)
{
    const bwPanel* panel = static_cast<bwPanel*>(panel_node.Widget());
    bwLayoutItem* layout = static_cast<bwLayoutItem*>(panel_node.Layout());
    assert(layout);
    const int32_t initial_width = layout->width;
    bwPoint panel_items_pos = panel_pos;

    layout->padding = padding;
    //  panel_items_pos.x += padding;
    panel_items_pos.y -= panel->header_height + padding;

    //  layout->width -= 2.0f * padding;

    layout->resolve(panel_node, panel_items_pos, item_margin, scale_fac);

    layout->width = initial_width;
}

void bwLayoutItem::resolve(bwScreenGraph::bwNode& node,
                           const bwPoint& layout_pos,
                           const uint32_t item_margin,
                           const float scale_fac)
{
    bwScreenGraph::bwNode::ChildList* children = node.Children();
    int32_t xpos = layout_pos.x + padding;
    int32_t ypos = layout_pos.y - padding;
    int32_t item_width_base = 0;
    // Width calculations may have imprecisions so widths don't add up to full layout width. We
    // 'fix' this by adding a pixel to each layout-item until the remainder is 0, meaning total
    // width matches parent width precisely. Also makes layout less jaggy on window size changes.
    int32_t additional_remainder_x = 0;

    location = layout_pos;

    if (children == nullptr)
    {
        return;
    }

    height = 0;

    if (flow_direction == FLOW_DIRECTION_HORIZONTAL)
    {
        const uint32_t margin_count = countNeededMargins(*children);
        const uint32_t width_no_margins = width - (margin_count * item_margin) - (2 * padding);
        // The max-width of each item is the max-width of the parent, divided by the count of
        // its horizontally placed sub-items. In other words, each item has the same max-width
        // that add up to the parent's max-width. From this max-width, the item margins are
        // subtracted.
        uint32_t tot_row_cols = countRowColumns(*children);

        if (tot_row_cols == 0)
        {
            width = 0;
        }
        else
        {
            item_width_base = width_no_margins / tot_row_cols;
            additional_remainder_x = width_no_margins % tot_row_cols;
        }
        assert(additional_remainder_x >= 0);
    }
    else
    {
        item_width_base = width - (2 * padding);
    }

    for (auto node_iter = children->begin(); node_iter != children->end(); ++node_iter)
    {
        bwScreenGraph::bwNode& child_node = **node_iter;
        bwLayoutItem* layout = static_cast<bwLayoutItem*>(child_node.Layout());
        bwWidget* widget = child_node.Widget();
        int32_t item_width = item_width_base;

        if (!child_node.isVisible())
        {
            continue;
        }

        const bwScreenGraph::bwNode* next = getNextUnhiddenNode(node_iter);

        // Simple correction for precision issues.
        if (additional_remainder_x > 0)
        {
            if (next)
            {
                item_width++;
                additional_remainder_x--;
            }
            else
            {
                // For the last button in a row, additional_remainder_x may be
                // larger than 1, so add all that is left to it.
                //				assert(additional_remainder_x <= 2);
                item_width += additional_remainder_x;
                additional_remainder_x = 0;
            }
        }

        if (widget && widget_cast<bwPanel>(widget))
        {
            bwPanel& panel = static_cast<bwPanel&>(*widget);

            assert(layout);
            layout->width = item_width;
            layout->height = 0;

            panel.header_height = panel.getHeaderHeightHint() * scale_fac;
            if (child_node.childrenVisible())
            {
                resolvePanelContents(
                    child_node, bwPoint(xpos, ypos), item_margin, item_margin, scale_fac);
                layout->height += 2 * item_margin;
            }
            location.y = ypos - layout->height;
            layout->height += panel.header_height;
        }
        else if (layout)
        {
            layout->width = item_width;
            layout->resolve(child_node, bwPoint(xpos, ypos), item_margin, scale_fac);
            location.y = ypos;
        }
        if (widget)
        {
            const int32_t widget_height = layout ? layout->height : widget->height_hint * scale_fac;
            widget->rectangle.set(xpos, item_width, ypos - widget_height, widget_height);
            location.y = widget->rectangle.ymin;
            if (align)
            {
                alignNode(node_iter, flow_direction);
            }
        }

        const int32_t child_width = child_node.Rectangle().width();
        const int32_t child_height = child_node.Rectangle().height();

        if (flow_direction == FLOW_DIRECTION_VERTICAL)
        {
            if (needsMarginAfterNode(node_iter, align))
            {
                ypos -= item_margin;
                height += item_margin;
            }
            else if (next)
            {
                ypos += 1;
                height -= 1;
            }
            ypos -= child_height;
            height += child_height;
        }
        else if (flow_direction == FLOW_DIRECTION_HORIZONTAL)
        {
            if (needsMarginAfterNode(node_iter, align))
            {
                xpos += item_margin;
            }
            else if (next)
            {
                xpos -= 1;
                additional_remainder_x += 1;
            }
            xpos += child_width;
            height = std::max(height, child_height);
        }
    }

    height += padding;
    if (flow_direction == FLOW_DIRECTION_HORIZONTAL)
    {
        height += padding;
    }

    // xpos should match right side of layout precisely now.
    assert((flow_direction != FLOW_DIRECTION_HORIZONTAL) || (xpos == layout_pos.x + width));
}

bwRectanglePixel bwLayoutItem::getRectangle()
{
    const int32_t xmin = int32_t(location.x);
    const int32_t ymin = int32_t(location.y);

    return bwRectanglePixel{ xmin, xmin + width, ymin, ymin + height };
}

auto bwLayoutItem::countRowColumns(const bwScreenGraph::bwNode::ChildList& children) const
    -> uint32_t
{
    uint32_t count_child_cols = 0;

    assert(type == bwLayoutItem::Type::ROW);

    for (auto& node : children)
    {
        const bwWidget* widget = node->Widget();
        const bwLayoutItem* layout = static_cast<bwLayoutItem*>(node->Layout());

        if (!node->isVisible())
        {
            continue;
        }
        if (widget)
        {
            count_child_cols++;
        }
        else if (layout && layout->type != bwLayoutItem::Type::ROW)
        {
            count_child_cols++;
        }
        else if (layout)
        {
            count_child_cols += layout->countRowColumns(*node->Children());
        }
    }

    return count_child_cols;
}

/**
 * Calculate how many margins need to be added in between child-items of this layout.
 */
auto bwLayoutItem::countNeededMargins(const bwScreenGraph::bwNode::ChildList& children) const
    -> uint32_t
{
    uint32_t tot_margins = 0;

    for (auto child_iter = children.begin(); child_iter != children.end(); ++child_iter)
    {
        const bwScreenGraph::bwNode& child = **child_iter;

        if (!child.isVisible())
        {
            continue;
        }
        if (needsMarginAfterNode(child_iter, align))
        {
            tot_margins++;
        }
    }

    return tot_margins;
}

bwColumnLayout::bwColumnLayout(const bool align)
    : bwLayoutItem(bwLayoutItem::Type::COLUMN, align, FLOW_DIRECTION_VERTICAL)
{
}

bwRowLayout::bwRowLayout(const bool align)
    : bwLayoutItem(bwLayoutItem::Type::ROW, align, FLOW_DIRECTION_HORIZONTAL)
{
}

bwPanelLayout::bwPanelLayout() : bwLayoutItem(bwLayoutItem::Type::PANEL, false, FLOW_DIRECTION_VERTICAL)
{
}

bwScrollViewLayout::bwScrollViewLayout()
    : bwLayoutItem(bwLayoutItem::Type::SCROLL_VIEW, false, FLOW_DIRECTION_VERTICAL)
{
}

void bwScrollViewLayout::resolve(bwScreenGraph::bwNode& node,
                                 const bwPoint& layout_pos,
                                 const uint32_t item_margin,
                                 const float scale_fac)
{
    bwWidget* widget = node.Widget();
    bwScrollView* view_widget = widget_cast<bwScrollView>(widget);

    if (!widget || !view_widget)
    {
        assert(false);
        return;
    }

    // Could check if layout actually needs to be updated.

    widget->rectangle.set(layout_pos.x, widget->width_hint, layout_pos.y, widget->height_hint);

    bwRectanglePixel content_bounds = view_widget->getContentBounds(scale_fac);
    width = content_bounds.width();

    bwPoint children_pos{ float(content_bounds.xmin),
                          float(content_bounds.ymax + view_widget->getScrollOffsetY()) };

    bwLayoutItem::resolve(node, children_pos, item_margin, scale_fac);
    height += padding;
}

}  // namespace bWidgets
