#pragma once

#include "bwContainerWidget.h"

namespace bWidgets
{
	class bwMenuBar : public bwContainerWidget
	{
	public:
		bwMenuBar(const bwScreenGraph::bwContainerNode& node, uint32_t width);

		static constexpr std::string_view identifier = "bwMenuBar";

		void draw() override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override
		{
			return nullptr;
		}
	private:
		static uint32_t getHeightHint();
	};
}  // namespace bWidgets
