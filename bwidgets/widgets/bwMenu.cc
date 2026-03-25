#include "bwMenu.h"
#include "bwEvent.h"
#include "bwPainter.h"
#include "bwStyle.h"

namespace bWidgets {

bwMenuItem::bwMenuItem(std::string label, Type type) : label(std::move(label)), type(type)
{
}

class bwMenuHandler : public bwScreenGraph::EventHandler {
 public:
  explicit bwMenuHandler(bwMenu& menu) : menu(menu)
  {
  }

  void onMouseMove(bwEvent&) override
  {
  }
  void onMousePress(bwMouseButtonEvent& event) override;

 private:
  bwMenu& menu;
};

bwMenu::bwMenu(std::optional<unsigned int> width_hint, std::optional<unsigned int> height_hint)
    : bwContainerWidget(width_hint, height_hint)
{
}

auto bwMenu::getTypeIdentifier() const -> std::string_view
{
  return "bwMenu";
}

void bwMenu::draw(bwStyle& style)
{
  bwPainter painter;

  const bwGradient gradient{
      base_style.backgroundColor(), base_style.shadeTop(), base_style.shadeBottom()};
  painter.drawRoundboxWidgetBase(base_style, style, rectangle, gradient, base_style.corner_radius);

  unsigned int y = rectangle.ymin + item_padding;

  for (const auto& item : items) {
    bwRectanglePixel item_rect{
        rectangle.xmin, rectangle.xmax, static_cast<int>(y), static_cast<int>(y + item_height)};

    switch (item->type) {
      case bwMenuItem::Type::ACTION:
      case bwMenuItem::Type::SUBMENU:
        drawItem(style, *item, item_rect);
        break;
      case bwMenuItem::Type::SEPARATOR:
        drawSeparator(style, item_rect);
        break;
    }

    y += item_height;
  }
}

void bwMenu::drawItem(bwStyle& style, const bwMenuItem& item, const bwRectanglePixel& item_rect)
{
  bwPainter painter;

  if (item.type == bwMenuItem::Type::SUBMENU) {
    bwRectanglePixel arrow_rect = item_rect;
    arrow_rect.xmin = arrow_rect.xmax - item_rect.height();
    painter.active_drawtype = bwPainter::DrawType::FILLED;
    painter.setActiveColor(base_style.textColor());
    painter.drawTriangle(arrow_rect, Direction::RIGHT);
  }

  bwRectanglePixel text_rect = item_rect;
  text_rect.xmin += item_padding + 5;
  text_rect.xmax -= item_padding;

  bwColor text_col = base_style.textColor();
  if (!item.enabled) {
    text_col.shade(50u);
  }
  painter.setActiveColor(text_col);
  painter.drawText(item.label, text_rect, TextAlignment::LEFT);
}

void bwMenu::drawSeparator(bwStyle& style, const bwRectanglePixel& item_rect)
{
  bwPainter painter;

  const int sep_y = item_rect.ymin + (item_rect.height() / 2);
  painter.setActiveColor(base_style.borderColor());
  painter.drawLine(bwPoint(item_rect.xmin + item_padding, sep_y),
                   bwPoint(item_rect.xmax - item_padding, sep_y));
}

void bwMenu::registerProperties()
{
  bwContainerWidget::registerProperties();
}

auto bwMenu::createHandler() -> std::unique_ptr<bwScreenGraph::EventHandler>
{
  return std::make_unique<bwMenuHandler>(*this);
}

auto bwMenu::addItem(std::string label) -> bwMenuItem&
{
  items.emplace_back(std::make_unique<bwMenuItem>(std::move(label), bwMenuItem::Type::ACTION));
  return *items.back();
}

auto bwMenu::addSubmenu(std::string label) -> bwMenuItem&
{
  items.emplace_back(std::make_unique<bwMenuItem>(std::move(label), bwMenuItem::Type::SUBMENU));
  return *items.back();
}

auto bwMenu::addSeparator() -> bwMenuItem&
{
  items.emplace_back(std::make_unique<bwMenuItem>("", bwMenuItem::Type::SEPARATOR));
  return *items.back();
}

auto bwMenu::addItemToSubmenu(bwMenuItem& submenu, std::string label) -> bwMenuItem&
{
  submenu.submenu_items.emplace_back(
      std::make_unique<bwMenuItem>(std::move(label), bwMenuItem::Type::ACTION));
  return *submenu.submenu_items.back();
}

auto bwMenu::getItems() const -> const std::vector<std::unique_ptr<bwMenuItem>>&
{
  return items;
}

void bwMenuHandler::onMousePress(bwMouseButtonEvent& event)
{
  if (event.button != bwMouseButtonEvent::Button::LEFT) {
    return;
  }

  const bwPoint& pos = event.location;
  if (!menu.rectangle.isCoordinateInside(pos.x, pos.y)) {
    return;
  }

  unsigned int y = menu.rectangle.ymin + menu.item_padding;
  for (const auto& item : menu.items) {
    bwRectanglePixel item_rect{menu.rectangle.xmin,
                               menu.rectangle.xmax,
                               static_cast<int>(y),
                               static_cast<int>(y + menu.item_height)};

    if (item_rect.isCoordinateInside(pos.x, pos.y)) {
      if (item->type != bwMenuItem::Type::SEPARATOR && item->enabled) {
        event.swallow();
      }
      break;
    }

    y += menu.item_height;
  }
}

}  // namespace bWidgets