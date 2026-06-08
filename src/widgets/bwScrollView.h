#pragma once

#include <cmath>

#include "bwContainerWidget.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		class bwWidgetNode;
	}

	class bwScrollBar;

	class bwScrollView : public bwContainerWidget
	{
		friend class bwScrollViewHandler;

	public:
		bwScrollView(bwScreenGraph::bwContainerNode& node,
			uint32_t width = 0,
			uint32_t height = 0);

		static constexpr std::string_view identifier = "bwScrollView";

		void draw(bwStyle& style) override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		int32_t getScrollOffsetY() const;
		bwRectanglePixel getContentBounds(float interface_scale) const;

	private:
		bwScrollBar& getVerticalScrollBar() const;
		bwRectanglePixel getVerticalScrollbarRect(const bwStyle& style) const;
		void drawScrollBars(bwStyle& style);
		bool isScrollable() const;
		void validizeScrollValues();

		static int32_t getScrollbarWidth(float interface_scale);

		constexpr static int32_t SCROLL_BAR_SIZE = 17;

		std::unique_ptr<bwScreenGraph::bwWidgetNode> scrollbar_node;

		int32_t vert_scroll{ 0 };
	};

}  // namespace bWidgets
