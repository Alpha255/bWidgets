#pragma once

#include <string>

#include "bwAbstractButton.h"

namespace bWidgets
{
	class bwPushButton : public bwAbstractButton
	{
	public:
		bwPushButton(const std::string& text,
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		std::string_view getTypeIdentifier() const override;

		const bwIconInterface* getIcon() const override;
		bwPushButton& setIcon(const class bwIconInterface&);

		bool canAlign() const override;

	private:
		const class bwIconInterface* icon{ nullptr };
	};

}  // namespace bWidgets
