#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwGradient.h"
#include "screen/bwIconInterface.h"
#include "styling/bwStyle.h"

namespace bWidgets
{
	class bwPaintEngine;
	class bwPolygon;

	class bwPainter
	{
	public:
		enum class DrawType
		{
			FILLED,
			OUTLINE,
			LINE,
		};

		bwPainter();

		void drawPolygon(const bwPolygon& poly);
		void drawText(const std::string& text,
			const bwRectanglePixel& rectangle,
			const TextAlignment align) const;
		void drawIcon(const bwIconInterface&, const bwRectanglePixel&, const bwColor& color) const;
		void drawIcon(const bwIconInterface& interface, const bwRectanglePixel& rect) const
		{
			drawIcon(interface, rect, bwColor::White);
		}

		void setActiveColor(const bwColor& color);
		const bwColor& getActiveColor() const;
		const bwColor& getVertexColor(const size_t vertex_index) const;

		void setContentMask(const bwRectanglePixel& value);
		const bwRectanglePixel& getContentMask() const;

		void enableGradient(const bwGradient& gradient);
		bool isGradientEnabled() const;

		void drawTextAndIcon(const std::string& text,
			const bwIconInterface* icon,
			const bwRectanglePixel& rectangle,
			const TextAlignment alignment,
			const bwColor& color) const;
		void drawTextAndIcon(
			const std::string& text,
			const bwIconInterface* icon,
			const bwRectanglePixel& rectangle,
			const TextAlignment alignment) const
		{
			drawTextAndIcon(text, icon, rectangle, alignment, bwColor::White);
		}

		// Primitives
		void drawRoundbox(const bwRectanglePixel& rect, uint32_t corners, const float radius);
		void drawRectangle(const bwRectanglePixel& rect);
		void drawRoundboxWidgetBase(const bwWidgetStyle& style,
			const bwRectanglePixel& rectangle,
			const bwGradient& gradient);
		void drawCheckMark(const bwRectanglePixel& rect);
		void drawTriangle(const bwRectanglePixel& rect, Direction direction);
		void drawLine(const bwPoint& from, const bwPoint& to);

		static std::unique_ptr<bwPaintEngine> s_paint_engine;

		bool use_antialiasing{ false };
		DrawType active_drawtype;

	private:
		void fillVertexColorsWithGradient(const bwPolygon& polygon, const bwRectanglePixel& bounding_box);

		bwColor active_color;
		std::vector<bwColor> vert_colors;
		std::unique_ptr<bwGradient> active_gradient;
		bwRectanglePixel content_mask;
	};

}  // namespace bWidgets
