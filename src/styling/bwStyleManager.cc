#include <cassert>
#include <fstream>
#include <string>
#include <filesystem>
#include <cereal/archives/json.hpp>

#include "bwStyleManager.h"

namespace bWidgets
{
	bool bwStyleManager::s_initialized = false;

	std::filesystem::path getStyleFilePath(bwStyle::Type type)
	{
		static const std::filesystem::path root_path(RESOURCES_PATH_STR);

		switch (type)
		{
		case bwStyle::Type::DARK:
			return root_path / "style_dark.json";
		case bwStyle::Type::LIGHT:
			return root_path / "style_light.json";
		case bwStyle::Type::CUSTOM:
			return root_path / "style_custom.json";
		}
	}

	bwStyleManager& bwStyleManager::get()
	{
		static bwStyleManager instance;

		if (!s_initialized)
		{
			for (bwStyle::Type type = bwStyle::Type::DARK; type < bwStyle::Type::NUM;)
			{
				instance.load(type);

				if (instance.current_style == bwStyle::Type::NUM && instance.getStyle(type).in_use)
				{
					instance.current_style = type;
				}

				type = static_cast<bwStyle::Type>(static_cast<size_t>(type) + 1u);
			}

			s_initialized = true;
		}

		return instance;
	}

	void bwStyleManager::save(bwStyle::Type type)
	{
	}

	void bwStyleManager::load(bwStyle::Type type)
	{
	}

	void bwStyleManager::setStyle(bwStyle::Type type)
	{
		current_style = type;

		for (auto& callback : onSetStyleCallbacks) 
		{
			callback(type);
		}
	}

}  // namespace bWidgets
