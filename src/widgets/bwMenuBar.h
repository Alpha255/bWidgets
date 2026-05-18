#pragma once

#include "bwContainerWidget.h"

namespace bWidgets
{
	class bwMenuBar : public bwContainerWidget
	{
	public:
		bwMenuBar(const bwScreenGraph::bwContainerNode& node, uint32_t width);

		std::string_view getTypeIdentifier() const override;

		void draw(bwStyle& style) override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override
		{
			return nullptr;
		}
	private:
		static uint32_t getHeightHint();
	};
}  // namespace bWidgets
