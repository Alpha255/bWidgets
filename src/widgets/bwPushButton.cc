#include "bwPushButton.h"

namespace bWidgets
{
	bwPushButton::bwPushButton(const std::string& text,
		std::optional<uint32_t> width_hint,
		std::optional<uint32_t> height_hint)
		: bwAbstractButton(text, width_hint, height_hint)
	{
	}

	const bwIconInterface* bwPushButton::getIcon() const
	{
		return icon;
	}

	bwPushButton& bwPushButton::setIcon(const bwIconInterface& icon_interface)
	{
		icon = &icon_interface;
		return *this;
	}

	bool bwPushButton::canAlign() const
	{
		return true;
	}

}  // namespace bWidgets
