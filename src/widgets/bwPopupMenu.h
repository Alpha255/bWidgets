#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwContainerWidget.h"
#include "bwRectangle.h"
#include "bwWidgetBaseStyle.h"
#include "blender_icon_defines.h"
#include "event/bwEvent.h"

namespace bWidgets
{
	class bwMenuItem
	{
	public:
		enum class Type
		{
			ACTION,
			SUBMENU,
			SEPARATOR,
		};

		bwMenuItem(std::string inlabel,
			Type inType = Type::ACTION,
			bool inEnabled = true,
			BIFIconSvg inIcon = BIFIconSvg::ICON_NONE);

		Type type;
		bool enabled = true;
		bwInputKeys shortcut;
		std::string label;
		BIFIconSvg icon = BIFIconSvg::ICON_NONE;
	};

	class bwPopupMenu : public bwContainerWidget
	{
		friend class bwMenuHandler;
	public:
		bwPopupMenu(const bwScreenGraph::bwContainerNode& node,
			std::string inTitle,
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		std::string_view getTypeIdentifier() const override;

		void draw(bwStyle& style) override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		bwPopupMenu& addAction(std::string label, bool enabled = true, BIFIconSvg icon = BIFIconSvg::ICON_NONE);
		bwPopupMenu& addSubmenu(std::string label, bool enabled = true, BIFIconSvg icon = BIFIconSvg::ICON_NONE);
		bwPopupMenu& addSeparator();

		bwRectanglePixel getDropdownRect() const;
		void drawDropdown(bwStyle& style);

		bool is_open{ false };

	private:
		void drawItem(bwStyle& style,
			const bwMenuItem& item,
			const bwRectanglePixel& rect,
			bool hovered);

		void drawSeparator(const bwRectanglePixel& rect);

		std::string title;
		std::vector<std::unique_ptr<bwMenuItem>> menu_items;

		static constexpr uint32_t item_padding = 4;
		static constexpr uint32_t item_height = 20;
		static constexpr uint32_t preferred_dropdown_width = 200;

		int32_t hovered_item{ -1 };

		bwWidgetBaseStyle item_style;
		bwWidgetBaseStyle item_hover_style;
	};
}  // namespace bWidgets
