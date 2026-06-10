#pragma once

#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

#include "bwColor.h"

namespace bWidgets
{
	enum RoundboxCorner
	{
		NONE = 0,
		BOTTOM_LEFT = (1 << 0),
		BOTTOM_RIGHT = (1 << 1),
		TOP_LEFT = (1 << 2),
		TOP_RIGHT = (1 << 3),
		/* Convenience */
		ALL = (BOTTOM_LEFT | BOTTOM_RIGHT | TOP_LEFT | TOP_RIGHT),
	};

	enum class TextAlignment
	{
		LEFT,
		CENTER,
		RIGHT,
	};

	enum class Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

	struct bwWidgetStyle
	{
		bwColor background_color;
		bwColor text_color;
		bwColor border_color;
		bwColor decoration_color;

		int32_t shade_top = 0;
		int32_t shade_bottom = 0;  // TODO could make this a char - or support gradient for background_color even.
		TextAlignment text_alignment;  // Default value set in constructor.
		RoundboxCorner roundbox_corners;
		float corner_radius = 0.0f;  // TODO Utility function for calculating final radius (roundbox_radius * dpi_fac).

		inline float shadeTop() const { return shade_top / 255.0f; }
		inline float shadeBottom() const { return shade_bottom / 255.0f; }
		inline bool isBorderVisible() const { return background_color != border_color;}

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(background_color),
				CEREAL_NVP(text_color),
				CEREAL_NVP(border_color),
				CEREAL_NVP(decoration_color),
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
		void serialize(Archive& ar)
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
		inline const bwWidgetStyle* getWidgetStyle() const
		{
			auto it = widget_styles.find(std::string(Widget::identifier));
			return it == widget_styles.cend() ? nullptr : &it->second;
		}

		bool in_use = false;
		std::unordered_map<std::string, bwWidgetStyle> widget_styles;
	};

	template<class Widget>
	class bwWidgetStyleHandle
	{
	public:
		static inline const bwWidgetStyle& get()
		{
			assert(style);
			return *style;
		}

	protected:
		friend class bwStyleManager;

		static inline void setStyle(const bwWidgetStyle* in_style) { style = in_style; }
	private:
		static inline const bwWidgetStyle* style = nullptr;
	};

}  // namespace bWidgets
