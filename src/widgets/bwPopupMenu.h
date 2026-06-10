#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwContainerWidget.h"
#include "bwRectangle.h"
#include "blender_icon_defines.h"
#include "event/bwEvent.h"

namespace bWidgets
{
	class bwMenuItem
	{
	public:
		using onBuildPopupMenu = std::function<void(class bwPopupMenu&)>;

		enum class Type
		{
			ACTION,
			SUBMENU,
			SEPARATOR,
		};

		bwMenuItem(std::string inlabel,
			Type inType = Type::ACTION,
			bool inEnabled = true,
			BIFIconSvg inIcon = BIFIconSvg::ICON_NONE,
			bwShortcut inShortcut = bwShortcut());

		inline bwMenuItem& setEnabled(bool is_enabled) { enabled = is_enabled; return *this; }
		inline bool isEnabled() const { return enabled; }

		inline bool isSeparator() const { return type == Type::SEPARATOR; }
		inline bool isSubmenu() const { return type == Type::SUBMENU; }
		inline bool isAction() const { return type == Type::ACTION; }

		inline const std::string& getLabel() const { return label; }
		inline bwMenuItem& setLabel(std::string inLabel) { label = std::move(inLabel); return *this; }

		inline BIFIconSvg getIcon() const { return icon; }
		inline bwMenuItem& setIcon(BIFIconSvg inIcon) { icon = inIcon; return *this; }

		inline const bwShortcut& getShortcut() const { return shortcut; }
		inline bwMenuItem& setShortcut(const bwShortcut& inShortcut) { shortcut = inShortcut; return *this; }
		inline bwMenuItem& setShortcut(KeyboardKey primary) { shortcut = bwShortcut(primary); return *this; }
		inline bwMenuItem& setShortcut(ModifierKey first, KeyboardKey primary) { shortcut = bwShortcut(first, primary); return *this; }
		inline bwMenuItem& setShortcut(ModifierKey first, ModifierKey second, KeyboardKey primary) { shortcut = bwShortcut(first, second, primary); return *this; }
	private:
		friend class bwPopupMenu;

		inline void setOnBuildSubmenu(onBuildPopupMenu&& onBuildFunc) { on_build_submenu = std::move(onBuildFunc); }

		Type type;
		bool enabled = true;
		bwShortcut shortcut;
		std::string label;
		BIFIconSvg icon = BIFIconSvg::ICON_NONE;
		onBuildPopupMenu on_build_submenu;
	};

	class bwPopupMenu : public bwContainerWidget
	{
		friend class bwPopupMenuHandler;
	public:
		bwPopupMenu(const bwScreenGraph::bwContainerNode& node,
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		static constexpr std::string_view identifier = "bwPopupMenu";

		void draw() override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		bwMenuItem& addAction(std::string label);
		bwMenuItem& addSubmenu(std::string label, bwMenuItem::onBuildPopupMenu&& onBuildSubmenu);
		bwPopupMenu& addSeparator();

		bwRectanglePixel getDropdownRect() const;
		void drawDropdown(const bwWidgetStyle& style);
	private:
		void drawItem(const bwWidgetStyle& style,
			const bwMenuItem& item,
			const bwRectanglePixel& rect,
			bool hovered);

		void drawSeparator(const bwWidgetStyle& style, const bwRectanglePixel& rect);

		std::vector<std::unique_ptr<bwMenuItem>> menu_items;

		static constexpr uint32_t item_padding = 4;
		static constexpr uint32_t item_height = 20;
		static constexpr uint32_t preferred_dropdown_width = 200;

		int32_t hovered_item{ -1 };
	};
}  // namespace bWidgets
