#pragma once

#include "bwRectangle.h"

namespace bWidgets
{
	class bwLayoutInterface
	{
	public:
		virtual ~bwLayoutInterface() = default;

		virtual bwRectanglePixel getRectangle() = 0;

		virtual void resolve(bWidgets::bwScreenGraph::bwNode& /*node*/,
			const bWidgets::bwPoint& /*layout_pos*/,
			const uint32_t /*item_margin*/,
			const float /*scale_factor*/) 
		{
		}
	};

}  // namespace bWidgets
