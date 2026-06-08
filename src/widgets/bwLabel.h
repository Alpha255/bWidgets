#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"

namespace bWidgets
{
	class bwIconInterface;

	class bwLabel : public bwWidget
	{
	public:
		bwLabel(std::string text = "",
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		static constexpr std::string_view identifier = "bwLabel";

		void draw(bwStyle& style) override;
		void registerProperties() override;
		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		const std::string* getLabel() const override;
		bwLabel& setLabel(const std::string&);
		bwLabel& setIcon(const bwIconInterface& icon_interface);

	private:
		std::string text;
		const class bwIconInterface* icon{ nullptr };

		bwColor text_color;
	};

}  // namespace bWidgets
