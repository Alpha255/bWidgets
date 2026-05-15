#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwContainerWidget.h"
#include "bwRectangle.h"
#include "bwWidgetBaseStyle.h"

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

		bwMenuItem(std::string label, Type type = Type::ACTION);

		std::string label;
		std::string shortcut;
		Type type;
		bool enabled{ true };
		int32_t icon_id{ 0 };

		std::vector<std::unique_ptr<bwMenuItem>> submenu_items;
	};

	class bwMenu : public bwContainerWidget
	{
		friend class bwMenuHandler;

	public:
		bwMenu(const bwScreenGraph::bwContainerNode& node,
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		std::string_view getTypeIdentifier() const override;

		void draw(bwStyle& style) override;
		void registerProperties() override;
		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		bwMenuItem& addItem(std::string label);
		bwMenuItem& addSubmenu(std::string label);
		bwMenuItem& addSeparator();
		bwMenuItem& addItemToSubmenu(bwMenuItem& submenu, std::string label);

		const std::vector<std::unique_ptr<bwMenuItem>>& getItems() const;

		uint32_t getItemHeight() const
		{
			return item_height;
		}

		/** Returns the height of the collapsed title bar button only. */
		uint32_t getPreferredHeight() const
		{
			return item_height;
		}

		bwRectanglePixel getDropdownRect() const;
		void drawDropdown(bwStyle& style);

		/** Style for normal (non-hovered) menu items. Set by the active style. */
		bwWidgetBaseStyle item_style;
		/** Style for hovered/selected menu items. Set by the active style. */
		bwWidgetBaseStyle item_hover_style;

		/** Menu title shown in the menu bar button (e.g., "File"). */
		std::string label{};
		/** Whether the dropdown panel is currently open. */
		bool is_open{ false };
		/** Title bar rectangle saved by Stage::draw() before any expansion. */
		bwRectanglePixel title_rect{};
		/** Preferred dropdown width in pixels, computed by Stage from item text
		 * widths. */
		int32_t preferred_dropdown_width{ 160 };
		/** Width of the title bar button, computed by Stage from the label text
		 * width. */
		int32_t preferred_label_width{ 60 };

	private:
		void drawItem(bwStyle& style,
			const bwMenuItem& item,
			const bwRectanglePixel& item_rect,
			bool hovered);
		void drawSeparator(const bwRectanglePixel& item_rect);

		std::vector<std::unique_ptr<bwMenuItem>> items;
		uint32_t item_height{ 20 };
		uint32_t item_padding{ 5 };
		int32_t hovered_item{ -1 };
	};

}  // namespace bWidgets
