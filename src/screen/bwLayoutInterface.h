#pragma once

#include "bwRectangle.h"

namespace bWidgets
{
	class bwLayoutInterface
	{
	public:
		virtual ~bwLayoutInterface() = default;

		virtual bwRectanglePixel getRectangle() = 0;

		virtual bwPoint getLayoutLocation(const bwRectangle<float>& layout_rect) const
		{
			return bwPoint(layout_rect.xmin, layout_rect.ymax);
		}

		virtual void resolve(bwScreenGraph::bwNode& node,
			const bwRectangle<float>& layout_rect,
			const float scale_factor) 
		{
			return resolve(node, getLayoutLocation(layout_rect), scale_factor);
		}

		virtual void resolve(bwScreenGraph::bwNode& /*node*/,
			const bwPoint& /*layout_pos*/,
			const float /*scale_factor*/)
		{
		}
	};

}  // namespace bWidgets
