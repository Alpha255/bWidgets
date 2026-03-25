#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwContainerWidget.h"

namespace bWidgets {

class bwMenuItem {
 public:
  enum class Type {
    ACTION,
    SUBMENU,
    SEPARATOR,
  };

  bwMenuItem(std::string label, Type type = Type::ACTION);

  std::string label;
  Type type;
  bool enabled{true};
  int icon_id{0};

  std::vector<std::unique_ptr<bwMenuItem>> submenu_items;
};

class bwMenu : public bwContainerWidget {
  friend class bwMenuHandler;

 public:
  bwMenu(std::optional<unsigned int> width_hint = std::nullopt,
         std::optional<unsigned int> height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(bwStyle& style) override;
  void registerProperties() override;
  auto createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  auto addItem(std::string label) -> bwMenuItem&;
  auto addSubmenu(std::string label) -> bwMenuItem&;
  auto addSeparator() -> bwMenuItem&;
  auto addItemToSubmenu(bwMenuItem& submenu, std::string label) -> bwMenuItem&;

  auto getItems() const -> const std::vector<std::unique_ptr<bwMenuItem>>&;

  unsigned int getItemHeight() const
  {
    return item_height;
  }

 private:
  void drawItem(bwStyle& style, const bwMenuItem& item, const bwRectanglePixel& item_rect);
  void drawSeparator(bwStyle& style, const bwRectanglePixel& item_rect);

  std::vector<std::unique_ptr<bwMenuItem>> items;
  unsigned int item_height{20};
  unsigned int item_padding{5};
};

}  // namespace bWidgets