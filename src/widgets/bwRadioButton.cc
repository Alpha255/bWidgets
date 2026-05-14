#include "bwRadioButton.h"
#include "event/bwEvent.h"

namespace bWidgets
{

bwRadioButton::bwRadioButton(const std::string& text,
                             std::optional<uint32_t> width_hint,
                             std::optional<uint32_t> height_hint)
    : bwAbstractButton(text, width_hint, height_hint)
{
}

std::string_view bwRadioButton::getTypeIdentifier() const
{
    return "bwRadioButton";
}

bool bwRadioButton::canAlign() const
{
    return true;
}

// ------------------ Handling ------------------

class bwRadioButtonHandler : public bwAbstractButtonHandler
{
public:
    bwRadioButtonHandler(bwRadioButton& button);
    ~bwRadioButtonHandler() = default;

    void onMousePress(bwMouseButtonEvent&) override;
    void onMouseRelease(bwMouseButtonEvent&) override;
};

bwRadioButtonHandler::bwRadioButtonHandler(bwRadioButton& button) : bwAbstractButtonHandler(button)
{
}

std::unique_ptr<bwScreenGraph::bwEventHandler> bwRadioButton::createHandler()
{
    return std::make_unique<bwRadioButtonHandler>(*this);
}

void bwRadioButtonHandler::onMousePress(bwMouseButtonEvent& event)
{
    if (event.button == bwMouseButtonEvent::Button::LEFT)
    {
        button.setState(bwWidget::State::SUNKEN);
        apply();
        event.swallow();
    }
}

void bwRadioButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
{
    event.swallow();
}

}  // namespace bWidgets
