#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <cereal/archives/json.hpp>

#include "bwStyleManager.h"
#include "widgets/bwAbstractButton.h"
#include "widgets/bwCheckbox.h"
#include "widgets/bwContainerWidget.h"
#include "widgets/bwLabel.h"
#include "widgets/bwMenuBar.h"
#include "widgets/bwMenuButton.h"
#include "widgets/bwNumberSlider.h"
#include "widgets/bwPanel.h"
#include "widgets/bwPopupMenu.h"
#include "widgets/bwPushButton.h"
#include "widgets/bwRadioButton.h"
#include "widgets/bwScrollBar.h"
#include "widgets/bwScrollView.h"
#include "widgets/bwTextBox.h"

namespace bWidgets
{
	bool bwStyleManager::s_initialized = false;

	bwStyleManager::bwStyleManager()
	{
		for (uint32_t index = 0u; index < styles.size(); ++index)
		{
			styles[index].reset(new bwStyle(static_cast<bwStyle::Type>(index)));
		}
	}

	void bwStyleManager::createDefaultWidgetStyles(bwStyle::Type type)
	{
#define REGISTER_DEFAULT_WIDGET_STYLE(Type) getStyle(type).registerWidgetStyle<Type>()

		REGISTER_DEFAULT_WIDGET_STYLE(bwAbstractButton);
		REGISTER_DEFAULT_WIDGET_STYLE(bwContainerWidget);
		REGISTER_DEFAULT_WIDGET_STYLE(bwCheckbox);
		REGISTER_DEFAULT_WIDGET_STYLE(bwLabel);
		REGISTER_DEFAULT_WIDGET_STYLE(bwMenuBar);
		REGISTER_DEFAULT_WIDGET_STYLE(bwMenuButton);
		REGISTER_DEFAULT_WIDGET_STYLE(bwNumberSlider);
		REGISTER_DEFAULT_WIDGET_STYLE(bwPanel);
		REGISTER_DEFAULT_WIDGET_STYLE(bwPopupMenu);
		REGISTER_DEFAULT_WIDGET_STYLE(bwPushButton);
		REGISTER_DEFAULT_WIDGET_STYLE(bwRadioButton);
		REGISTER_DEFAULT_WIDGET_STYLE(bwScrollBar);
		REGISTER_DEFAULT_WIDGET_STYLE(bwScrollView);
		REGISTER_DEFAULT_WIDGET_STYLE(bwTextBox);

		save(type);

#undef REGISTER_DEFAULT_WIDGET_STYLE
	}

	std::filesystem::path getStyleFilePath(bwStyle::Type type)
	{
		static const std::filesystem::path root_path(RESOURCES_PATH_STR);

		switch (type)
		{
		default:
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

			if (instance.current_style == bwStyle::Type::NUM)
			{
				instance.current_style = bwStyle::Type::DARK;
			}

			s_initialized = true;
		}

		return instance;
	}

	void bwStyleManager::save(bwStyle::Type type)
	{
		auto path = getStyleFilePath(type);
		std::ofstream file(path);
		cereal::JSONOutputArchive archive(file);
		archive(
			cereal::make_nvp(bwStyle::identifier.data(), getStyle(type))
		);
	}

	void bwStyleManager::load(bwStyle::Type type)
	{
		auto path = getStyleFilePath(type);
		if (!std::filesystem::exists(path))
		{
			createDefaultWidgetStyles(type);
		}

		std::ifstream file(path);
		cereal::JSONInputArchive archive(file);
		archive(
			cereal::make_nvp(bwStyle::identifier.data(), getStyle(type))
		);
	}

	void bwStyleManager::setStyle(bwStyle::Type type)
	{
		current_style = type;
		getStyle(type).in_use = true;

		for (auto& callback : onSetStyleCallbacks)
		{
			callback(type);
		}
	}

	template<class Widget>
	struct bwWidgetStyleRegister
	{
		bwWidgetStyleRegister()
		{
			bwStyleManager::get().registerWidgetStyle<Widget>();
		}
	};

#define WIDGET_STYLE_STATIC_REGISTER(Type) \
		static bwWidgetStyleRegister<Type> s_##Type##_style_register;

	WIDGET_STYLE_STATIC_REGISTER(bwAbstractButton);
	WIDGET_STYLE_STATIC_REGISTER(bwContainerWidget);
	WIDGET_STYLE_STATIC_REGISTER(bwCheckbox);
	WIDGET_STYLE_STATIC_REGISTER(bwLabel);
	WIDGET_STYLE_STATIC_REGISTER(bwMenuBar);
	WIDGET_STYLE_STATIC_REGISTER(bwMenuButton);
	WIDGET_STYLE_STATIC_REGISTER(bwNumberSlider);
	WIDGET_STYLE_STATIC_REGISTER(bwPanel);
	WIDGET_STYLE_STATIC_REGISTER(bwPopupMenu);
	WIDGET_STYLE_STATIC_REGISTER(bwPushButton);
	WIDGET_STYLE_STATIC_REGISTER(bwRadioButton);
	WIDGET_STYLE_STATIC_REGISTER(bwScrollBar);
	WIDGET_STYLE_STATIC_REGISTER(bwScrollView);
	WIDGET_STYLE_STATIC_REGISTER(bwTextBox);

}  // namespace bWidgets
