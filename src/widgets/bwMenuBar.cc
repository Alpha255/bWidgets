#include "bwMenuBar.h"
#include "styling/bwPreferences.h"

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
	}

	uint32_t bwMenuBar::getHeightHint()
	{
		return bwUserPreferences::get().widget_unit;
	}
}
