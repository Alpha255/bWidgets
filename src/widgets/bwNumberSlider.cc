#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "event/bwEvent.h"
#include "paint/bwPainter.h"
#include "bwNumberSlider.h"
#include "styling/bwStyleManager.h"

namespace bWidgets
{
	bwNumberSlider::bwNumberSlider(std::optional<uint32_t> width_hint,
		std::optional<uint32_t> height_hint)
		: bwTextBox(width_hint, height_hint), precision(2)
	{
	}

	void bwNumberSlider::draw()
	{
		auto& style = getStyle<bwNumberSlider>();
		const float scale_factor = bwStyleManager::get().getCurrentStyle().scale_factor;

		bwPainter painter;
		bwRectanglePixel inner_rect = rectangle;
		const float radius = style.corner_radius * scale_factor;

		// Inner - "inside" of outline, so scale down
		inner_rect.resize(-1);

		painter.setContentMask(inner_rect);

		painter.enableGradient(bwGradient(style.background_color, style.shadeTop(), style.shadeBottom()));
		painter.drawRoundbox(inner_rect, style.roundbox_corners, radius - 1.0f);

		painter.active_drawtype = bwPainter::DrawType::FILLED;

		// Text editing
		if (is_text_editing)
		{
			// Selection drawing
			painter.setActiveColor(style.decoration_color);
			painter.drawRectangle(selection_rectangle);
		}
		else
		{
			drawValueIndicator(painter, style, scale_factor);
		}

		// Outline
		painter.setActiveColor(style.border_color);
		painter.active_drawtype = bwPainter::DrawType::OUTLINE;
		painter.drawRoundbox(rectangle, style.roundbox_corners, radius);

		painter.setActiveColor(style.text_color);
		if (!is_text_editing)
		{
			painter.drawText(text, rectangle, style.text_alignment);
		}
		painter.drawText(valueToString(precision),
			rectangle,
			is_text_editing ? TextAlignment::LEFT : TextAlignment::RIGHT);
	}

	void bwNumberSlider::drawValueIndicator(bwPainter& painter, const bwWidgetStyle& style, const float scale) const
	{
		bwGradient gradient = bwGradient(style.decoration_color,
			// shadeTop/Bottom intentionally inverted
			style.shadeBottom(),
			style.shadeTop());
		bwRectanglePixel indicator_offset_rect = rectangle;
		bwRectanglePixel indicator_rect = rectangle;
		uint32_t roundbox_corners = style.roundbox_corners;
		const float radius = style.corner_radius * scale;
		float right_side_radius = radius;

		indicator_offset_rect.xmax = indicator_offset_rect.xmin + right_side_radius;

		indicator_rect.xmin = indicator_offset_rect.xmax;
		indicator_rect.xmax = indicator_rect.xmin + calcValueIndicatorWidth(style, scale);
		if (indicator_rect.xmax > (rectangle.xmax - right_side_radius))
		{
			right_side_radius *= (indicator_rect.xmax + right_side_radius - rectangle.xmax) / right_side_radius;
		}
		else
		{
			roundbox_corners &= ~(TOP_RIGHT | BOTTOM_RIGHT);
		}

		painter.enableGradient(gradient);
		painter.drawRoundbox(indicator_offset_rect, roundbox_corners & ~(TOP_RIGHT | BOTTOM_RIGHT), radius);
		painter.drawRoundbox(indicator_rect, roundbox_corners & ~(TOP_LEFT | BOTTOM_LEFT), right_side_radius);
	}

	bwNumberSlider& bwNumberSlider::setValue(float _value)
	{
		const int32_t precision_fac = std::pow(10, precision);
		const float unclamped_value = std::max(min, std::min(max, _value));

		value = std::roundf(unclamped_value * precision_fac) / precision_fac;
		return *this;
	}

	float bwNumberSlider::getValue() const
	{
		return value;
	}

	bwNumberSlider& bwNumberSlider::setMinMax(float _min, float _max)
	{
		min = _min;
		max = _max;
		return *this;
	}

	std::string bwNumberSlider::valueToString(uint32_t precision) const
	{
		std::stringstream string_stream;
		string_stream << std::fixed << std::setprecision(precision) << value;
		return string_stream.str();
	}

	float bwNumberSlider::calcValueIndicatorWidth(const bwWidgetStyle& style, const float scale) const
	{
		const float range = max - min;
		const float radius = style.corner_radius * scale;

		assert(max > min);
		return ((value - min) * (rectangle.width() - radius)) / range;
	}

	// ------------------ Handling ------------------

	class bwNumberSliderHandler : public bwTextBoxHandler
	{
	public:
		bwNumberSliderHandler(bwNumberSlider& numberslider);
		~bwNumberSliderHandler() = default;

		void onMousePress(bwMouseButtonEvent&) override;
		void onMouseRelease(bwMouseButtonEvent&) override;
		void onMouseClick(bwMouseButtonEvent&) override;
		void onMouseDrag(bwMouseButtonDragEvent&) override;

	private:
		bwNumberSlider& numberslider;

		// Initial value before starting to edit.
		float initial_value;
	};

	bwNumberSliderHandler::bwNumberSliderHandler(bwNumberSlider& numberslider)
		: bwTextBoxHandler(numberslider)
		, numberslider(numberslider)
		, initial_value(0.0f)
	{
	}

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwNumberSlider::createHandler()
	{
		return std::make_unique<bwNumberSliderHandler>(*this);
	}

	void bwNumberSliderHandler::onMousePress(bwMouseButtonEvent& event)
	{
		if (event.button == bwMouseButtonEvent::Button::LEFT)
		{
			initial_value = numberslider.value;
			numberslider.setState(bwWidget::State::SUNKEN);

			event.swallow();
		}
		else if (event.button == bwMouseButtonEvent::Button::RIGHT)
		{
			if (numberslider.is_text_editing)
			{
				endTextEditing();
			}
			else if (is_dragging)
			{
				numberslider.value = initial_value;
			}

			event.swallow();
		}
	}

	void bwNumberSliderHandler::onMouseRelease(bwMouseButtonEvent& event)
	{
		if (is_dragging)
		{
			numberslider.setState(bwWidget::State::NORMAL);
		}
		is_dragging = false;

		event.swallow();
	}

	void bwNumberSliderHandler::onMouseClick(bwMouseButtonEvent& event)
	{
		if (event.button == bwMouseButtonEvent::Button::LEFT)
		{
			startTextEditing();
		}

		event.swallow();
	}

	void bwNumberSliderHandler::onMouseDrag(bwMouseButtonDragEvent& event)
	{
		if (event.button == bwMouseButtonEvent::Button::LEFT)
		{
			numberslider.setValue(initial_value + (event.drag_distance.x / (float)numberslider.rectangle.width()));
			if (numberslider.apply_functor)
			{
				(*numberslider.apply_functor)();
			}

			is_dragging = true;

			event.swallow();
		}
	}

}  // namespace bWidgets
