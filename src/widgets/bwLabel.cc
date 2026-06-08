#include <cmath>

#include "paint/bwPainter.h"
#include "bwStyle.h"

#include "bwLabel.h"

namespace bWidgets
{
	bwLabel::bwLabel(std::string text,
		std::optional<uint32_t> width_hint,
		std::optional<uint32_t> height_hint)
		: bwWidget(width_hint, height_hint), text(std::move(text))
	{
		initialize();
	}

	void bwLabel::draw(bwStyle& style)
	{
		bwPainter painter;

		painter.setContentMask(rectangle);
		painter.setActiveColor(text_color);
		painter.drawTextAndIcon(text, icon, rectangle, TextAlignment::LEFT, style.dpi_fac);
	}

	void bwLabel::registerProperties()
	{
		style_properties.addColor("color", text_color);
	}

	const std::string* bwLabel::getLabel() const
	{
		return &text;
	}

	bwLabel& bwLabel::setLabel(const std::string& label)
	{
		text = label;
		return *this;
	}

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwLabel::createHandler()
	{
		return nullptr;
	}

	bwLabel& bwLabel::setIcon(const bwIconInterface& icon_interface)
	{
		icon = &icon_interface;
		return *this;
	}

}  // namespace bWidgets
