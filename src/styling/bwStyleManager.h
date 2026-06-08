#pragma once

#include <array>
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <assert.h>

#include "bwStyle.h"
#include "stylesheet/bwStyleSheet.h"
#include "bwStyleProperties.h"
#include "styling/bwWidgetBaseStyle.h"

namespace bWidgets
{
	/**
	 * \brief Singleton class for managing styles through type ID's.
	 */
	class bwStyleManager
	{
	public:
		using StyleTypeArray = std::array<bwStyle::StyleType, int32_t(bwStyle::TypeID::NUM)>;

		static bwStyleManager& get();

		static std::unique_ptr<bwStyle> createStyleFromTypeID(bwStyle::TypeID type_id);

		void registerDefaultStyleTypes();

		const StyleTypeArray& getBuiltinStyleTypes() const;

		template<class Widget>
		void registerStyle(bwStyle::TypeID type_id)
		{
			assert(type_id < bwStyle::TypeID::NUM);

			auto& style = getStyles(type_id)[Widget::identifier];
			auto& properties = getProperties(type_id).emplace_back();
			style.setProperties(&properties);

			bwStyleHandle<Widget>::onRegisterStyleProperties(type_id, properties);

			registerOnSetStyleCallback<Widget>([](bwStyle::TypeID type_id) {
				auto& style = getStyles(type_id)[Widget::identifier];
				bwStyleHandle<Widget>::setStyle(&style);
			});
		}

		template<class Widget>
		const bwWidgetStyle* getStyle(bwStyle::TypeID type_id) const
		{
			assert(type_id < bwStyle::TypeID::NUM);

			auto it = getStyles(type_id).find(Widget::identifier);
			return it == getStyles(type_id).end() ? nullptr : &it->second;
		}

		void save(bwStyle::TypeID type_id);
		void load(bwStyle::TypeID type_id);

		void setStyle(bwStyle::TypeID type_id);
		inline bwStyle::TypeID getStyle() const { return current_style; }
	protected:
		inline std::unordered_map<std::string_view, bwWidgetStyle>& getStyles(bwStyle::TypeID type_id)
		{
			assert(type_id < bwStyle::TypeID::NUM);
			return styles[static_cast<size_t>(type_id)];
		}

		inline std::vector<bwStyleProperties>& getProperties(bwStyle::TypeID type_id)
		{
			assert(type_id < bwStyle::TypeID::NUM);
			return properties[static_cast<size_t>(type_id)];
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
		bwStyleManager() = default;
		bwStyleManager(bwStyleManager const&) = delete;

		void operator=(bwStyleManager const&) = delete;

		StyleTypeArray builtin_style_types;
		//	std::vector<StyleType> custom_types;

		bwStyle::TypeID current_style = bwStyle::TypeID::CLASSIC;

		std::array<std::unique_ptr<bwStyle>, (size_t)bwStyle::TypeID::NUM - 1u> builtin_stypes;

		std::array<std::unordered_map<std::string_view, bwWidgetStyle>, (size_t)bwStyle::TypeID::NUM> styles;
		std::array<std::vector<bwStyleProperties>, (size_t)bwStyle::TypeID::NUM> properties;
		std::vector<onSetStyleCallback> onSetStyleCallbacks;
	};

}  // namespace bWidgets
