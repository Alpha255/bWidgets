#pragma once

#include <array>
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <assert.h>

#include "bwStyle.h"

namespace bWidgets
{
	/**
	 * \brief Singleton class for managing styles through type ID's.
	 */
	class bwStyleManager
	{
	public:
		static bwStyleManager& get();

		template<class Widget>
		inline void registerWidgetStyle()
		{
			onSetStyleCallback callback = [this](bwStyle::Type type) {
				auto style = getWidgetStyle<Widget>(type);
				bwWidgetStyleHandle<Widget>::setStyle(style);
			};
			callback(current_style);

			registerOnSetStyleCallback<Widget>(std::move(callback));
		}

		template<class Widget>
		inline const bwWidgetStyle* getWidgetStyle(bwStyle::Type type) const
		{
			assert(type < bwStyle::Type::NUM);
			return getStyle(type).getWidgetStyle<Widget>();
		}

		void setStyle(bwStyle::Type type);
		inline const bwStyle& getCurrentStyle() const { return getStyle(current_style); }
	protected:
		void save(bwStyle::Type type);
		void load(bwStyle::Type type);

		static bool s_initialized;

		inline const bwStyle& getStyle(bwStyle::Type type) const
		{
			assert(type < bwStyle::Type::NUM);
			return *styles[static_cast<size_t>(type)];
		}

		inline bwStyle& getStyle(bwStyle::Type type)
		{
			assert(type < bwStyle::Type::NUM);
			return *styles[static_cast<size_t>(type)];
		}

		using onSetStyleCallback = std::function<void(bwStyle::Type)>;

		template<class Widget>
		inline void registerOnSetStyleCallback(onSetStyleCallback&& callback)
		{
			onSetStyleCallbacks.emplace_back(std::move(callback));
		}
	private:
		bwStyleManager() = default;
		bwStyleManager(bwStyleManager const&) = delete;
		void operator=(bwStyleManager const&) = delete;

		bwStyle::Type current_style = bwStyle::Type::NUM;

		std::array<std::unique_ptr<bwStyle>, (size_t)bwStyle::Type::NUM> styles;
		std::vector<onSetStyleCallback> onSetStyleCallbacks;
	};

}  // namespace bWidgets
