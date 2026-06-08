#include <cassert>
#include <cstring>
#include <fstream>

#include "bwStyleClassic.h"
#include "bwStyleFlatDark.h"
#include "bwStyleFlatGrey.h"
#include "bwStyleFlatLight.h"

#include "bwStyleManager.h"

namespace bWidgets
{
	bwStyleManager& bwStyleManager::get()
	{
		static bwStyleManager instance;
		return instance;
	}

	std::unique_ptr<bwStyle> bwStyleManager::createStyleFromTypeID(bwStyle::TypeID type)
	{
		using std::make_unique;

		switch (type)
		{
		case bwStyle::TypeID::CLASSIC:
			return make_unique<bwStyleClassic>();
		case bwStyle::TypeID::CLASSIC_CSS:
			return make_unique<bwStyleCSS>();
		case bwStyle::TypeID::FLAT_GREY:
			return make_unique<bwStyleFlat>();
		case bwStyle::TypeID::FLAT_DARK:
			return make_unique<bwStyleFlatDark>();
		case bwStyle::TypeID::FLAT_LIGHT:
			return make_unique<bwStyleFlatLight>();
		default:
			assert(0);
			return nullptr;
		}
	}

	void bwStyleManager::registerDefaultStyleTypes()
	{
		builtin_style_types[int32_t(bwStyle::TypeID::CLASSIC)].type_id = bwStyle::TypeID::CLASSIC;
		builtin_style_types[int32_t(bwStyle::TypeID::CLASSIC)].name = "Classic";

		builtin_style_types[int32_t(bwStyle::TypeID::CLASSIC_CSS)].type_id = bwStyle::TypeID::CLASSIC_CSS;
		builtin_style_types[int32_t(bwStyle::TypeID::CLASSIC_CSS)].name = "Classic (CSS)";

		builtin_style_types[int32_t(bwStyle::TypeID::FLAT_GREY)].type_id = bwStyle::TypeID::FLAT_GREY;
		builtin_style_types[int32_t(bwStyle::TypeID::FLAT_GREY)].name = "Flat Grey";

		builtin_style_types[int32_t(bwStyle::TypeID::FLAT_DARK)].type_id = bwStyle::TypeID::FLAT_DARK;
		builtin_style_types[int32_t(bwStyle::TypeID::FLAT_DARK)].name = "Flat Dark (CSS)";

		builtin_style_types[int32_t(bwStyle::TypeID::FLAT_LIGHT)].type_id = bwStyle::TypeID::FLAT_LIGHT;
		builtin_style_types[int32_t(bwStyle::TypeID::FLAT_LIGHT)].name = "Flat Light (CSS)";
	}

	const bwStyleManager::StyleTypeArray& bwStyleManager::getBuiltinStyleTypes() const
	{
		return builtin_style_types;
	}

	void bwStyleManager::save(bwStyle::TypeID type_id)
	{
		auto prev = current_style;
		current_style = type_id;
		saveToCss(std::string(builtin_style_types[int32_t(type_id)].name) + ".css");
		current_style = prev;
	}

	void bwStyleManager::load(bwStyle::TypeID type_id)
	{
		auto prev = current_style;
		current_style = type_id;
		loadFromCss(std::string(builtin_style_types[int32_t(type_id)].name) + ".css");
		current_style = prev;
	}

	static std::string readStyleTypeFromCss(std::string_view path)
	{
		std::ifstream file(path.data());
		std::string line;
		while (std::getline(file, line)) {
			const char* tag = "@bwStyleType ";
			size_t pos = line.find(tag);
			if (pos != std::string::npos) {
				auto start = pos + strlen(tag);
				auto end = line.find(" */", start);
				return line.substr(start, end - start);
			}
		}
		return {};
	}

	static bwStyle::TypeID typeIDFromName(const bwStyleManager::StyleTypeArray& types,
	                                       const std::string& name)
	{
		for (size_t i = 0; i < types.size(); i++) {
			if (types[i].name == name) {
				return types[i].type_id;
			}
		}
		return bwStyle::TypeID::NUM;
	}

	void bwStyleManager::saveToCss(std::string_view path)
	{
		std::ofstream file(std::string{path});
		if (!file.is_open()) {
			return;
		}

		file << "/* @bwStyleType "
		     << builtin_style_types[int32_t(current_style)].name
		     << " */\n\n";

		for (auto& [identifier, style] : getStyles(current_style)) {
			const auto* props = style.getProperties();
			if (!props) {
				continue;
			}

			file << identifier << " {\n";
			for (const auto& prop : *props) {
				file << "    " << prop->getIdentifier() << ": "
				     << prop->getValueString() << ";\n";
			}
			file << "}\n\n";
		}
	}

	void bwStyleManager::loadFromCss(std::string_view path)
	{
		std::string type_name = readStyleTypeFromCss(path);
		bwStyle::TypeID file_type = typeIDFromName(builtin_style_types, type_name);

		bwStyleSheet stylesheet(path);

		auto type_id = (file_type != bwStyle::TypeID::NUM) ? file_type : current_style;

		for (auto& [identifier, style] : getStyles(type_id))
		{
			auto* props = const_cast<bwStyleProperties*>(style.getProperties());
			if (!props)
			{
				continue;
			}
			for (auto& prop : *props)
			{
				stylesheet.resolveValue(identifier, bwWidget::State::NORMAL, *prop);
			}
		}

		for (auto& callback : onSetStyleCallbacks)
		{
			callback(type_id);
		}
	}

	void bwStyleManager::setStyle(bwStyle::TypeID type_id)
	{
		current_style = type_id;
		load(type_id);

		for (auto& callback : onSetStyleCallbacks) {
			callback(type_id);
		}
	}

}  // namespace bWidgets
