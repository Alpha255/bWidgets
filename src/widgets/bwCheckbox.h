#pragma once

#include "bwAbstractButton.h"

namespace bWidgets
{

class bwCheckbox : public bwAbstractButton
{
public:
    bwCheckbox(const std::string& text = "",
               bool useIcon = false,
               std::optional<uint32_t> width_hint = std::nullopt,
               std::optional<uint32_t> height_hint = std::nullopt);

    std::string_view getTypeIdentifier() const override;

    void draw(class bwStyle& style) override;

    std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

    bool isChecked() const;

private:
    bool isInsideCheckbox(const bwPoint& point) const;

    bwRectanglePixel getCheckboxRectangle() const;
    bwRectanglePixel getTextRectangle(const bwRectanglePixel& checkbox_rectangle) const;

    bool use_icon;
};

}  // namespace bWidgets
