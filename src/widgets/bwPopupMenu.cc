#include "bwPopupMenu.h"
#include "event/bwEvent.h"
#include "paint/bwPainter.h"
#include "bwStyle.h"

namespace bWidgets
{
	bwMenuItem::bwMenuItem(std::string inlabel,
		Type inType,
		bool inEnabled,
		BIFIconSvg inIcon)
		: label(std::move(inlabel))
		, type(inType)
		, enabled(inEnabled)
		, icon(inIcon)
	{
	}

	std::string_view bwPopupMenu::getTypeIdentifier() const
	{
		return "bwPopupMenu";
	}

	//bwRectanglePixel bwPopupMenu::getDropdownRect() const
	//{
	//	const int32_t dropdown_height = 2 * (int32_t)item_padding + (int32_t)items.size() * (int32_t)item_height;
	//	return bwRectanglePixel
	//	{
	//		title_rect.xmin,
	//		title_rect.xmin + preferred_dropdown_width,
	//		title_rect.ymin - dropdown_height,
	//		title_rect.ymin 
	//	};
	//}

	/** Draws the title bar button only (called by the normal Drawer pass). */
	void bwPopupMenu::draw(bwStyle& style)
	{
#if 1
#else
		bwPainter painter;

		/* Button rect is limited to the label width, not the full widget/screen width. */
		const bwRectanglePixel btn_rect
		{
			title_rect.xmin, 
			title_rect.xmin + preferred_label_width, 
			title_rect.ymin, 
			title_rect.ymax
		};

		/* Draw highlight background when open. */
		if (is_open)
		{
			const bwGradient hover_gradient
			{ 
				item_hover_style.backgroundColor(), 
				0.0f, 
				0.0f 
			};
			painter.drawRoundboxWidgetBase(item_hover_style, style, btn_rect, hover_gradient, item_hover_style.corner_radius);
		}

		/* Draw the menu label text. */
		bwRectanglePixel text_rect = btn_rect;
		text_rect.xmin += (int32_t)item_padding + 4;
		text_rect.xmax -= (int32_t)item_padding;
		const bwColor text_col = is_open ? item_hover_style.textColor() : item_style.textColor();
		painter.setActiveColor(text_col);
		painter.drawText(label, text_rect, TextAlignment::LEFT);
#endif
	}

	/** Draws the dropdown panel with all items (called as an overlay after the main draw pass). */
	void bwPopupMenu::drawDropdown(bwStyle& style)
	{
		//bwPainter painter;
		//bwRectanglePixel dropdown_rect = getDropdownRect();

		///* Draw dropdown background panel. */
		//const bwGradient gradient{ base_style.backgroundColor(),
		//						   base_style.shadeTop(),
		//						   base_style.shadeBottom() };
		//painter.drawRoundboxWidgetBase(base_style, style, dropdown_rect, gradient, base_style.corner_radius);

		///* Draw items from top (ymax) downward so the first item appears nearest the menu bar. */
		//int32_t y = dropdown_rect.ymax - (int32_t)item_padding;
		//int32_t item_index = 0;

		//for (const auto& item : items)
		//{
		//	y -= (int32_t)item_height;
		//	bwRectanglePixel item_rect
		//	{
		//		dropdown_rect.xmin, 
		//		dropdown_rect.xmax, 
		//		y, 
		//		y + (int32_t)item_height
		//	};

		//	switch (item->type)
		//	{
		//	case bwMenuItem::Type::ACTION:
		//	case bwMenuItem::Type::SUBMENU:
		//		drawItem(style, *item, item_rect, item_index == hovered_item);
		//		break;
		//	case bwMenuItem::Type::SEPARATOR:
		//		drawSeparator(item_rect);
		//		break;
		//	}

		//	item_index++;
		//}
	}

	void bwPopupMenu::drawItem(bwStyle& style,
		const bwMenuItem& item,
		const bwRectanglePixel& item_rect,
		bool hovered)
	{
		//bwPainter painter;
		//const bool can_highlight = hovered && item.enabled;

		///* Draw hover highlight. */
		//if (can_highlight)
		//{
		//	const float side_padding = item_height * 0.125f;
		//	bwRectanglePixel highlight_rect = item_rect;
		//	highlight_rect.xmin += static_cast<int32_t>(side_padding);
		//	highlight_rect.xmax -= static_cast<int32_t>(side_padding);

		//	const bwGradient hover_gradient
		//	{ 
		//		item_hover_style.backgroundColor(), 
		//		0.0f, 
		//		0.0f 
		//	};
		//	painter.drawRoundboxWidgetBase(item_hover_style,
		//		style,
		//		highlight_rect,
		//		hover_gradient,
		//		item_hover_style.corner_radius);
		//}

		///* Draw submenu arrow (right-aligned). */
		//if (item.type == bwMenuItem::Type::SUBMENU)
		//{
		//	const int32_t arrow_size = item_rect.height() * 2 / 3;
		//	const int32_t arrow_margin = (item_rect.height() - arrow_size) / 2;
		//	bwRectanglePixel arrow_rect{ item_rect.xmax - static_cast<int32_t>(item_padding) - arrow_size,
		//								 item_rect.xmax - static_cast<int32_t>(item_padding),
		//								 item_rect.ymin + arrow_margin,
		//								 item_rect.ymax - arrow_margin };

		//	painter.active_drawtype = bwPainter::DrawType::FILLED;
		//	painter.setActiveColor(can_highlight ? item_hover_style.textColor() : item_style.decorationColor());
		//	painter.drawTriangle(arrow_rect, Direction::RIGHT);
		//}

		///* Text rectangle. */
		//bwRectanglePixel text_rect = item_rect;
		//text_rect.xmin += static_cast<int32_t>(item_padding) + 5;
		//text_rect.xmax -= static_cast<int32_t>(item_padding) + (item.type == bwMenuItem::Type::SUBMENU ? item_rect.height() : 0);

		///* Text color. */
		//bwColor text_col;
		//if (!item.enabled)
		//{
		//	const float* tc = item_style.textColor().getColor();
		//	text_col = bwColor(tc[0] * 0.5f, tc[1] * 0.5f, tc[2] * 0.5f, tc[3]);
		//}
		//else if (can_highlight)
		//{
		//	text_col = item_hover_style.textColor();
		//}
		//else
		//{
		//	text_col = item_style.textColor();
		//}

		//painter.setActiveColor(text_col);
		//painter.drawText(item.label, text_rect, TextAlignment::LEFT);

		///* Shortcut text (right-aligned, dimmed when not hovered). */
		//if (!item.shortcut.empty())
		//{
		//	bwColor shortcut_col = text_col;
		//	if (!can_highlight)
		//	{
		//		const float* tc = text_col.getColor();
		//		shortcut_col = bwColor(tc[0] * 0.7f, tc[1] * 0.7f, tc[2] * 0.7f, tc[3]);
		//	}
		//	painter.setActiveColor(shortcut_col);
		//	painter.drawText(item.shortcut, text_rect, TextAlignment::RIGHT);
		//}
	}

	void bwPopupMenu::drawSeparator(const bwRectanglePixel& item_rect)
	{
		//bwPainter painter;

		//const int32_t sep_y = item_rect.ymin + (item_rect.height() / 2);
		//const float* tc = item_style.textColor().getColor();
		//painter.setActiveColor(bwColor(tc[0], tc[1], tc[2], 0.12f));
		//painter.drawLine(bwPoint(item_rect.xmin + item_padding, sep_y), bwPoint(item_rect.xmax - item_padding, sep_y));
	}

	class bwMenuHandler : public bwScreenGraph::bwEventHandler
	{
	public:
		explicit bwMenuHandler(bwPopupMenu& menu)
			: menu(menu)
		{
		}

		void onMouseMove(bwEvent& event) override
		{

		}

		void onMouseLeave(bwEvent&) override
		{

		}

		void onMousePress(bwMouseButtonEvent& event) override
		{

		}

	private:
		bwPopupMenu& menu;
	};

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwPopupMenu::createHandler()
	{
		return std::make_unique<bwMenuHandler>(*this);
	}
}  // namespace bWidgets
