#pragma once

#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
#include <cereal/cereal.hpp>

#include "bwColor.h"
#include "bwWidget.h"

namespace bWidgets
{
	struct bwWidgetStyle
	{
		inline const bwColor& backgroundColor(bwWidget::State state) const
		{
			assert(state < bwWidget::State::STATE_TOT);
			return background_colors[static_cast<size_t>(state)];
		}

		inline const bwColor& textColor(bwWidget::State state) const
		{
			assert(state < bwWidget::State::STATE_TOT);
			return text_colors[static_cast<size_t>(state)];
		}

		inline const bwColor& borderColor(bwWidget::State state) const
		{
			assert(state < bwWidget::State::STATE_TOT);
			return border_colors[static_cast<size_t>(state)];
		}

		inline const bwColor& decorationColor(bwWidget::State state) const
		{
			assert(state < bwWidget::State::STATE_TOT);
			return decoration_colors[static_cast<size_t>(state)];
		}

		float shadeTop() const;
		float shadeBottom() const;

		bool isBorderVisible() const;

		static constexpr size_t NUM_STATES = static_cast<size_t>(bwWidget::State::STATE_TOT);

		std::array<bwColor, NUM_STATES> background_colors;
		std::array<bwColor, NUM_STATES> text_colors;
		std::array<bwColor, NUM_STATES> border_colors;
		std::array<bwColor, NUM_STATES> decoration_colors;

		int32_t shade_top = 0;
		int32_t shade_bottom = 0;  // TODO could make this a char - or support gradient for background_color even.
		enum TextAlignment text_alignment;  // Default value set in constructor.
		enum RoundboxCorner roundbox_corners;
		float corner_radius = 0.0f;  // TODO Utility function for calculating final radius (roundbox_radius * dpi_fac).

		template<class Archive>
		void serialize(Archive ar)
		{
			ar(
				CEREAL_NVP(background_colors),
				CEREAL_NVP(text_colors),
				CEREAL_NVP(border_colors),
				CEREAL_NVP(decoration_colors),
				CEREAL_NVP(shade_top),
				CEREAL_NVP(shade_bottom),
				CEREAL_NVP(text_alignment),
				CEREAL_NVP(roundbox_corners),
				CEREAL_NVP(corner_radius)
			);
		} 
	};

	struct bwStyle
	{
		enum class Type
		{
			DARK,
			LIGHT,
			CUSTOM,
			NUM
		};

		Type type;
		float scale_factor = 1.0f;

		template<class Archive>
		void serialize(Archive ar)
		{
			ar(
				CEREAL_NVP(type),
				CEREAL_NVP(scale_factor),
				CEREAL_NVP(in_use),
				CEREAL_NVP(widget_styles)
			);
		}
	private:
		friend class bwStyleManager;

		bwStyle(Type in_type)
			: type(in_type)
		{
		}

		template<class Widget>
		inline const bwWidgetStyle* getWidgetStyle()
		{
			auto it = widget_styles.find(Widget::identifier);
			return it == widget_styles.cend() ? nullptr : &it->second;
		}

		bool in_use = false;
		std::unordered_map<std::string, bwWidgetStyle> widget_styles;
	};

	template<class Widget>
	class bwWidgetStyleHandle
	{
	public:
		static inline const struct bwWidgetStyle& get()
		{
			assert(style);
			return *style;
		}

	protected:
		friend class bwStyleManager;

		static inline void setStyle(struct bwWidgetStyle* in_style) { style = in_style; }
	private:
		static inline struct bwWidgetStyle* style = nullptr;
	};

}  // namespace bWidgets
