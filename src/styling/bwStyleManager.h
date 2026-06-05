#pragma once

#include <array>
#include <memory>

#include "bwStyle.h"

namespace bWidgets
{
	/**
	 * \brief Singleton class for managing styles through type ID's.
	 */
	class bwStyleManager
	{
	public:
		using StyleTypeArray = std::array<bwStyle::StyleType, int32_t(bwStyle::TypeID::NUM)>;

		static bwStyleManager& getStyleManager();
		static std::unique_ptr<bwStyle> createStyleFromTypeID(bwStyle::TypeID type_id);

		void registerDefaultStyleTypes();

		const StyleTypeArray& getBuiltinStyleTypes() const;

	private:
		bwStyleManager() = default;
		bwStyleManager(bwStyleManager const&) = delete;

		void operator=(bwStyleManager const&) = delete;

		StyleTypeArray builtin_style_types;
		//	std::vector<StyleType> custom_types;
	};

}  // namespace bWidgets
