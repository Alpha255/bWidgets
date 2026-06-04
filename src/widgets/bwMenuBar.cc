#include "bwMenuBar.h"
#include "paint/bwPainter.h"
#include "styling/bwPreferences.h"
#include "screen_graph/bwNode.h"

namespace bWidgets
{
	bwMenuBar::bwMenuBar(const bwScreenGraph::bwContainerNode& node, uint32_t width)
		: bwContainerWidget(node, width, getHeightHint())
	{
	}

	std::string_view bwMenuBar::getTypeIdentifier() const
	{
		return "bwMenuBar";
	}

	void bwMenuBar::draw(bwStyle& style)
	{
		bwPainter painter;
		painter.drawRectangle(rectangle);

		for (auto& child : *node.Children())
		{
			auto widget = child->Widget();
			assert(widget);
			widget->draw(style);
		}
	}

	uint32_t bwMenuBar::getHeightHint()
	{
		return bwUserPreferences::get().widget_unit;
	}
}
