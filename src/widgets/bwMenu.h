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
    int icon_id{ 0 };

    std::vector<std::unique_ptr<bwMenuItem>> submenu_items;
};

class bwMenu : public bwContainerWidget
{
    friend class bwMenuHandler;

public:
    bwMenu(const bwScreenGraph::bwContainerNode& node,
        std::optional<unsigned int> width_hint = std::nullopt,
        std::optional<unsigned int> height_hint = std::nullopt);

    auto getTypeIdentifier() const -> std::string_view override;

    void draw(bwStyle& style) override;
    void registerProperties() override;
    auto createHandler() -> std::unique_ptr<bwScreenGraph::bwEventHandler> override;

    auto addItem(std::string label) -> bwMenuItem&;
    auto addSubmenu(std::string label) -> bwMenuItem&;
    auto addSeparator() -> bwMenuItem&;
    auto addItemToSubmenu(bwMenuItem& submenu, std::string label) -> bwMenuItem&;

    auto getItems() const -> const std::vector<std::unique_ptr<bwMenuItem>>&;

    unsigned int getItemHeight() const
    {
        return item_height;
    }

    /** Returns the height of the collapsed title bar button only. */
    unsigned int getPreferredHeight() const
    {
        return item_height;
    }

    auto getDropdownRect() const -> bwRectanglePixel;
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
    int preferred_dropdown_width{ 160 };
    /** Width of the title bar button, computed by Stage from the label text
     * width. */
    int preferred_label_width{ 60 };

private:
    void drawItem(bwStyle& style,
                  const bwMenuItem& item,
                  const bwRectanglePixel& item_rect,
                  bool hovered);
    void drawSeparator(const bwRectanglePixel& item_rect);

    std::vector<std::unique_ptr<bwMenuItem>> items;
    unsigned int item_height{ 20 };
    unsigned int item_padding{ 5 };
    int hovered_item{ -1 };
};

}  // namespace bWidgets
