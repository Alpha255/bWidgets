#pragma once

#include "Stage.h"

namespace bWidgetsDemo
{
	class BlenderStage : public Stage
	{
	public:
		BlenderStage(uint32_t mask_width, uint32_t mask_height)
			: Stage(mask_width, mask_height)
		{
		}
	protected:
		std::unique_ptr<bWidgets::bwScreenGraph::bwScreenGraph> createScreenGraph(uint32_t width, uint32_t height) override;
		void buildWidgets() override;
	};
}