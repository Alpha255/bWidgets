#pragma once

#include "bwStyleSheet.h"
#include "bwStyle.h"
#include "bwStyleProperties.h"
#include "styling/bwWidgetBaseStyle.h"
#include <assert.h>

namespace bWidgets
{
	class bwStyleSheetManager
	{
	public:
		static bwStyleSheetManager& get();

		template<class Widget>
		void registerWidgetStyle(bwStyle::TypeID type_id)
		{
			assert(type_id < bwStyle::TypeID::NUM);

			auto& style = getWidgetStyles(type_id)[Widget::getTypeIdentifier()];
			auto& properties = getWidgetStyleProperties(type_id).emplace_back();
			Widget::onRegisterWidgetStyleProperties(type_id, properties);
			Widget::setStyle(&style);

			registerOnSetStyleCallback<Widget>([](bwStyle::TypeID type_id) {
				auto& style = getWidgetStyles(type_id)[Widget::getTypeIdentifier()];
				Widget::setStyle(&style);
			});
		}

		template<class Widget>
		const bwWidgetStyle* getWidgetStyle(bwStyle::TypeID type_id) const
		{
			assert(type_id < bwStyle::TypeID::NUM);

			std::string_view identifier = Widget::getTypeIdentifier();
			auto it = getWidgetStyles(type_id).find(identifier);
			return it == getWidgetStyles(type_id).end() ? nullptr : &it->second;
		}

		void save(bwStyle::TypeID type_id);
		void load(bwStyle::TypeID type_id);

		void setStyle(bwStyle::TypeID type_id);
		inline bwStyle::TypeID getStyle() const { return current_style_type; }
	protected:
		inline std::unordered_map<std::string_view, bwWidgetStyle>& getWidgetStyles(bwStyle::TypeID type_id)
		{
			assert(type_id < bwStyle::TypeID::NUM);
			return widget_styles[static_cast<size_t>(type_id)];
		}

		inline std::vector<bwStyleProperties>& getWidgetStyleProperties(bwStyle::TypeID type_id)
		{
			assert(type_id < bwStyle::TypeID::NUM);
			return widget_style_properties[static_cast<size_t>(type_id)];
		}

		void saveToCss(std::string_view path);
		void loadFromCss(std::string_view path);

		using onSetStyleCallback = std::function<void(bwStyle::TypeID)>;

		template<class Widget>
		void registerOnSetStyleCallback(onSetStyleCallback&& callback)
		{
			onSetStyleCallbacks.emplace_back(std::move(callback));
		}
	private:
		std::array<std::unordered_map<std::string_view, bwWidgetStyle>, (size_t)bwStyle::TypeID::NUM> widget_styles;
		std::array<std::vector<bwStyleProperties>, (size_t)bwStyle::TypeID::NUM> widget_style_properties;
		std::vector<onSetStyleCallback> onSetStyleCallbacks;
		bwStyle::TypeID current_style_type = bwStyle::TypeID::CLASSIC;
	};
}
