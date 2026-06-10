#include "event/bwEvent.h"
#include "paint/bwPainter.h"
#include "bwStyle.h"

#include "bwTextBox.h"

namespace bWidgets
{
	bwTextBox::bwTextBox(std::optional<uint32_t> width_hint,
		std::optional<uint32_t> height_hint)
		: bwWidget(width_hint, height_hint)
		, selection_rectangle(bwRectanglePixel())
	{
		initialize();
	}

	void bwTextBox::draw()
	{
		auto& style = getStyle<bwTextBox>();

		bwRectanglePixel inner_rect = rectangle;
		const bwGradient gradient
		{ 
			style.background_color,
			style.shadeTop(),
			style.shadeBottom()
		};
		bwPainter painter;

		painter.drawRoundboxWidgetBase(style, inner_rect, gradient);

		// Text editing
		if (is_text_editing && !selection_rectangle.isEmpty())
		{
			// Selection drawing
			painter.active_drawtype = bwPainter::DrawType::FILLED;
			painter.setActiveColor(style.decoration_color);
			painter.drawRectangle(selection_rectangle);
		}
		painter.setActiveColor(style.text_color);
		painter.drawText(text, rectangle, style.text_alignment);
	}

	bwTextBox& bwTextBox::setText(const std::string& value)
	{
		text = value;
		return *this;
	}

	const std::string_view bwTextBox::getLabel() const
	{
		return std::string_view(text);
	}

	bool bwTextBox::canAlign() const
	{
		return true;
	}

	// ------------------ Handling ------------------

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwTextBox::createHandler()
	{
		return std::make_unique<bwTextBoxHandler>(*this);
	}

	bwTextBoxHandler::bwTextBoxHandler(bwTextBox& textbox) 
		: textbox(textbox)
	{
	}

	void bwTextBoxHandler::startTextEditing()
	{
		textbox.setState(bwWidget::State::SUNKEN);
		textbox.is_text_editing = true;
	}

	void bwTextBoxHandler::endTextEditing()
	{
		textbox.setState(bwWidget::State::NORMAL);
		textbox.is_text_editing = false;
	}

	void bwTextBoxHandler::onMouseEnter(bwEvent&)
	{
		if (textbox.getState() == bwWidget::State::NORMAL)
		{
			textbox.setState(bwWidget::State::HIGHLIGHTED);
		}
	}

	void bwTextBoxHandler::onMouseLeave(bwEvent&)
	{
		if (textbox.getState() == bwWidget::State::HIGHLIGHTED)
		{
			textbox.setState(bwWidget::State::NORMAL);
		}
	}
	void bwTextBoxHandler::onMousePress(bwMouseButtonEvent& event)
	{
		if (event.button == bwMouseButtonEvent::Button::LEFT)
		{
			startTextEditing();
			event.swallow();
		}
		else if (event.button == bwMouseButtonEvent::Button::RIGHT)
		{
			if (textbox.getState() == bwWidget::State::SUNKEN)
			{
				endTextEditing();
				event.swallow();
			}
		}
	}

}  // namespace bWidgets
