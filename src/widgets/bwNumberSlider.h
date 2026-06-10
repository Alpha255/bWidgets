#pragma once

#include "bwTextBox.h"

namespace bWidgets
{
	class bwPainter;

	class bwNumberSlider : public bwTextBox
	{
	public:
		friend class bwNumberSliderHandler;

		bwNumberSlider(std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		static constexpr std::string_view identifier = "bwNumberSlider";

		void draw() override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		bwNumberSlider& setValue(float value);
		float getValue() const;
		bwNumberSlider& setMinMax(float min, float max);

		std::unique_ptr<bwFunctorInterface> apply_functor{ nullptr };

	private:
		std::string valueToString(uint32_t precision) const;
		void drawValueIndicator(bwPainter& painter, const bwWidgetStyle& style, const float scale) const;
		float calcValueIndicatorWidth(const bwWidgetStyle& style, const float scale) const;

		/**
		 * Support multiple numeric types. bwNumberSlider could be made
		 * a template class for this, but using union is just fine.
		 */

		union
		{
			// float
			struct
			{
				float value;
				float min, max;
				uint32_t precision;
			};
			// struct { int32_t value; ...}
			// struct { char value; ...}
			// ...
		};
	};

}  // namespace bWidgets
