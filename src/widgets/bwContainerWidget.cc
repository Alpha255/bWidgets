#include "bwContainerWidget.h"

namespace bWidgets
{

bwContainerWidget::bwContainerWidget(const bwScreenGraph::bwContainerNode& node,
                                     std::optional<uint32_t> width_hint,
                                     std::optional<uint32_t> height_hint)
    : bwWidget(width_hint, height_hint), node(node)
{
}

bwRectanglePixel bwContainerWidget::getMaskRectangle()
{
    bwRectanglePixel maskrect = rectangle;

    if (base_style.isBorderVisible())
    {
        /* With a visible border, we have to exclude the border size from the
         * mask, so that content appears clipped by the border. */
        maskrect.resize(-1);
    }

    return maskrect;
}

bool bwContainerWidget::childrenVisible() const
{
    /* By default, all children are visible, can be overriden if the container
     * widget supports hiding children. */
    return true;
}

void bwContainerWidget::registerProperties()
{
    base_style.registerProperties(style_properties);
}

}  // namespace bWidgets
