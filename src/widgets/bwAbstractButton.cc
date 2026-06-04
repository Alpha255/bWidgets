#include "event/bwEvent.h"
#include "paint/bwPainter.h"
#include "bwStyle.h"

#include "bwAbstractButton.h"

namespace bWidgets
{
	bwAbstractButton::bwAbstractButton(const std::string& text,
		std::optional<uint32_t> width_hint,
		std::optional<uint32_t> height_hint)
		: bwWidget(width_hint, height_hint), rounded_corners(RoundboxCorner::ALL),
		text(text)
	{
		initialize();
	}

	void bwAbstractButton::draw(bwStyle& style)
	{
		const bwGradient gradient
		{ 
			base_style.backgroundColor(),
			base_style.shadeTop(),
			base_style.shadeBottom()
		};
		bwPainter painter;

		painter.drawRoundboxWidgetBase(base_style, style, rectangle, gradient, base_style.corner_radius);

		// Text
		painter.setContentMask(rectangle);
#if 0
		painter.setActiveColor(base_style.textColor());
#else
		painter.setActiveColor(bwColor::White);
#endif
		painter.drawTextAndIcon(text, getIcon(), rectangle, base_style.text_alignment, style.dpi_fac);
	}

	void bwAbstractButton::registerProperties()
	{
		base_style.registerProperties(style_properties);
	}

	const std::string* bwAbstractButton::getLabel() const
	{
		return &text;
	}

	bwAbstractButton& bwAbstractButton::setLabel(const std::string& label)
	{
		text = label;
		return *this;
	}

	const bwIconInterface* bwAbstractButton::getIcon() const
	{
		return nullptr;
	}

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwAbstractButton::createHandler()
	{
		return std::make_unique<bwAbstractButtonHandler>(*this);
	}

	// ------------------ Handling ------------------

	bwAbstractButtonHandler::bwAbstractButtonHandler(bwAbstractButton& button) 
		: button(button)
	{
	}

	void bwAbstractButtonHandler::onMouseEnter(bwEvent&)
	{
		if (button.getState() == bwWidget::State::NORMAL)
		{
			button.setState(bwWidget::State::HIGHLIGHTED);
		}
	}

	void bwAbstractButtonHandler::onMouseLeave(bwEvent&)
	{
		if (button.getState() == bwWidget::State::HIGHLIGHTED)
		{
			button.setState(bwWidget::State::NORMAL);
		}
	}

	void bwAbstractButtonHandler::onMousePress(bwMouseButtonEvent& event)
	{
		if (event.button == bwMouseButtonEvent::Button::LEFT)
		{
			button.setState(bwWidget::State::SUNKEN);
			event.swallow();
		}
	}

	void bwAbstractButtonHandler::onMouseRelease(bwMouseButtonEvent& event)
	{
		if ((event.button == bwMouseButtonEvent::Button::LEFT) &&
			(button.getState() == bwWidget::State::SUNKEN))
		{
			button.setState(bwWidget::State::NORMAL);

			event.swallow();
		}
	}

	void bwAbstractButtonHandler::apply()
	{
		if (button.apply_functor)
		{
			(*button.apply_functor)();
		}
	}

}  // namespace bWidgets
