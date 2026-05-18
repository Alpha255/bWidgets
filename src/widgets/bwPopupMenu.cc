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
		: type(inType)
		, enabled(inEnabled)
		, label(std::move(inlabel))
		, icon(inIcon)
	{
	}

	bwPopupMenu::bwPopupMenu(const bwScreenGraph::bwContainerNode& node,
		std::string inTitle,
		std::optional<uint32_t> width_hint,
		std::optional<uint32_t> height_hint)
		: bwContainerWidget(node, width_hint, height_hint)
		, title(std::move(inTitle))
	{
		initialize();

		item_style.background_color = bwColor(0.25f);
		item_style.text_color = bwColor(1.0f);
		item_style.corner_radius = 4.0f;

		item_hover_style.background_color = bwColor(0.3f, 0.5f, 1.0f, 0.8f);
		item_hover_style.text_color = bwColor(1.0f);
		item_hover_style.corner_radius = 4.0f;
	}

	std::string_view bwPopupMenu::getTypeIdentifier() const
	{
		return "bwPopupMenu";
	}

	bwPopupMenu& bwPopupMenu::addAction(std::string label, bool enabled, BIFIconSvg icon)
	{
		menu_items.push_back(std::make_unique<bwMenuItem>(std::move(label), bwMenuItem::Type::ACTION, enabled, icon));
		return *this;
	}

	bwPopupMenu& bwPopupMenu::addSubmenu(std::string label, bool enabled, BIFIconSvg icon)
	{
		menu_items.push_back(std::make_unique<bwMenuItem>(std::move(label), bwMenuItem::Type::SUBMENU, enabled, icon));
		return *this;
	}

	bwPopupMenu& bwPopupMenu::addSeparator()
	{
		menu_items.push_back(std::make_unique<bwMenuItem>(std::string(), bwMenuItem::Type::SEPARATOR));
		return *this;
	}

	bwRectanglePixel bwPopupMenu::getDropdownRect() const
	{
		const int32_t dropdown_height = 2 * static_cast<int32_t>(item_padding)
			+ static_cast<int32_t>(menu_items.size()) * static_cast<int32_t>(item_height);
		return bwRectanglePixel
		{
			rectangle.xmin,
			rectangle.xmin + static_cast<int32_t>(preferred_dropdown_width),
			rectangle.ymin - dropdown_height,
			rectangle.ymin
		};
	}

	void bwPopupMenu::draw(bwStyle& style)
	{
		bwPainter painter;

		const bwRectanglePixel btn_rect
		{
			rectangle.xmin,
			rectangle.xmax,
			rectangle.ymin,
			rectangle.ymax
		};

		if (is_open)
		{
			const bwGradient hover_gradient
			{
				base_style.backgroundColor(),
				base_style.shadeTop(),
				base_style.shadeBottom()
			};
			painter.drawRoundboxWidgetBase(base_style, style, btn_rect, hover_gradient, base_style.corner_radius);
		}

		bwRectanglePixel text_rect = btn_rect;
		text_rect.xmin += static_cast<int32_t>(item_padding) + 4;
		text_rect.xmax -= static_cast<int32_t>(item_padding);
		painter.setActiveColor(base_style.textColor());
		painter.drawText(title, text_rect, TextAlignment::LEFT);
	}

	void bwPopupMenu::drawDropdown(bwStyle& style)
	{
		bwPainter painter;
		bwRectanglePixel dropdown_rect = getDropdownRect();

		const bwGradient gradient{ base_style.backgroundColor(),
								   base_style.shadeTop(),
								   base_style.shadeBottom() };
		painter.drawRoundboxWidgetBase(base_style, style, dropdown_rect, gradient, base_style.corner_radius);

		int32_t y = dropdown_rect.ymax - static_cast<int32_t>(item_padding);
		int32_t item_index = 0;

		for (const auto& item : menu_items)
		{
			y -= static_cast<int32_t>(item_height);
			bwRectanglePixel item_rect
			{
				dropdown_rect.xmin,
				dropdown_rect.xmax,
				y,
				y + static_cast<int32_t>(item_height)
			};

			switch (item->type)
			{
			case bwMenuItem::Type::ACTION:
			case bwMenuItem::Type::SUBMENU:
				drawItem(style, *item, item_rect, item_index == hovered_item);
				break;
			case bwMenuItem::Type::SEPARATOR:
				drawSeparator(item_rect);
				break;
			}

			item_index++;
		}
	}

	void bwPopupMenu::drawItem(bwStyle& style,
		const bwMenuItem& item,
		const bwRectanglePixel& item_rect,
		bool hovered)
	{
		bwPainter painter;
		const bool can_highlight = hovered && item.enabled;

		if (can_highlight)
		{
			const float side_padding = static_cast<float>(item_height) * 0.125f;
			bwRectanglePixel highlight_rect = item_rect;
			highlight_rect.xmin += static_cast<int32_t>(side_padding);
			highlight_rect.xmax -= static_cast<int32_t>(side_padding);

			const bwGradient hover_gradient
			{
				item_hover_style.backgroundColor(),
				0.0f,
				0.0f
			};
			painter.drawRoundboxWidgetBase(item_hover_style,
				style,
				highlight_rect,
				hover_gradient,
				item_hover_style.corner_radius);
		}

		if (item.type == bwMenuItem::Type::SUBMENU)
		{
			const int32_t arrow_size = item_rect.height() * 2 / 3;
			const int32_t arrow_margin = (item_rect.height() - arrow_size) / 2;
			bwRectanglePixel arrow_rect{ item_rect.xmax - static_cast<int32_t>(item_padding) - arrow_size,
										 item_rect.xmax - static_cast<int32_t>(item_padding),
										 item_rect.ymin + arrow_margin,
										 item_rect.ymax - arrow_margin };

			painter.active_drawtype = bwPainter::DrawType::FILLED;
			painter.setActiveColor(can_highlight ? item_hover_style.textColor() : item_style.decorationColor());
			painter.drawTriangle(arrow_rect, Direction::RIGHT);
		}

		bwRectanglePixel text_rect = item_rect;
		text_rect.xmin += static_cast<int32_t>(item_padding) + 5;
		text_rect.xmax -= static_cast<int32_t>(item_padding)
			+ (item.type == bwMenuItem::Type::SUBMENU ? item_rect.height() : 0);

		bwColor text_col;
		if (!item.enabled)
		{
			const float* tc = item_style.textColor().getColor();
			text_col = bwColor(tc[0] * 0.5f, tc[1] * 0.5f, tc[2] * 0.5f, tc[3]);
		}
		else if (can_highlight)
		{
			text_col = item_hover_style.textColor();
		}
		else
		{
			text_col = item_style.textColor();
		}

		painter.setActiveColor(text_col);
		painter.drawText(item.label, text_rect, TextAlignment::LEFT);

		if (!item.shortcut.keys.empty())
		{
			bwColor shortcut_col = text_col;
			if (!can_highlight)
			{
				const float* tc = text_col.getColor();
				shortcut_col = bwColor(tc[0] * 0.7f, tc[1] * 0.7f, tc[2] * 0.7f, tc[3]);
			}
			painter.setActiveColor(shortcut_col);
			// TODO: Convert bwInputKeys to display string properly
			painter.drawText("...", text_rect, TextAlignment::RIGHT);
		}
	}

	void bwPopupMenu::drawSeparator(const bwRectanglePixel& item_rect)
	{
		bwPainter painter;

		const int32_t sep_y = item_rect.ymin + (item_rect.height() / 2);
		const float* tc = item_style.textColor().getColor();
		painter.setActiveColor(bwColor(tc[0], tc[1], tc[2], 0.12f));
		painter.drawLine(bwPoint(item_rect.xmin + static_cast<int32_t>(item_padding), sep_y),
			bwPoint(item_rect.xmax - static_cast<int32_t>(item_padding), sep_y));
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
			if (!menu.is_open)
			{
				return;
			}

			const bwRectanglePixel dropdown = menu.getDropdownRect();
			if (!dropdown.isCoordinateInside(event.location.x, event.location.y))
			{
				menu.hovered_item = -1;
				return;
			}

			const int32_t rel_y = dropdown.ymax - static_cast<int32_t>(menu.item_padding) - event.location.y;
			const int32_t idx = rel_y / static_cast<int32_t>(menu.item_height);

			if (idx >= 0 && static_cast<size_t>(idx) < menu.menu_items.size()
				&& menu.menu_items[idx]->type != bwMenuItem::Type::SEPARATOR)
			{
				menu.hovered_item = idx;
			}
			else
			{
				menu.hovered_item = -1;
			}
		}

		void onMouseLeave(bwEvent&) override
		{
			menu.hovered_item = -1;
		}

		void onMousePress(bwMouseButtonEvent& event) override
		{
			if (event.button != bwMouseButtonEvent::Button::LEFT)
			{
				return;
			}

			if (!menu.is_open)
			{
				menu.is_open = true;
				event.swallow();
				return;
			}

			const bwRectanglePixel dropdown = menu.getDropdownRect();
			if (dropdown.isCoordinateInside(event.location.x, event.location.y))
			{
				if (menu.hovered_item >= 0
					&& static_cast<size_t>(menu.hovered_item) < menu.menu_items.size())
				{
					const auto& item = menu.menu_items[menu.hovered_item];
					if (item->enabled && item->type == bwMenuItem::Type::ACTION)
					{
						// Action triggered -- TODO: fire callback
					}
				}
				menu.is_open = false;
				menu.hovered_item = -1;
				event.swallow();
			}
			else
			{
				menu.is_open = false;
				menu.hovered_item = -1;
			}
		}

	private:
		bwPopupMenu& menu;
	};

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwPopupMenu::createHandler()
	{
		return std::make_unique<bwMenuHandler>(*this);
	}
}  // namespace bWidgets
