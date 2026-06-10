#pragma once

#include "bwContainerWidget.h"

namespace bWidgets
{
	class bwPanel : public bwContainerWidget
	{
		friend class bwPanelHandler;

	public:
		enum class State
		{
			OPEN,
			CLOSED,
		};

		bwPanel(const bwScreenGraph::bwContainerNode& node,
			std::string label,
			std::optional<uint32_t> header_height_hint = std::nullopt);

		static constexpr std::string_view identifier = "bwPanel";

		void draw() override;

		const std::string_view getLabel() const override;
		bool childrenVisible() const override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		uint32_t getHeaderHeightHint() const;

		uint32_t header_height;
		State panel_state{ State::OPEN };

	private:
		void drawHeader(const bwWidgetStyle& style, const float scale) const;
		bwRectanglePixel getHeaderRectangle() const;
		bool isCoordinateInsideHeader(const bwPoint& point) const;

		std::string label;

	public:
		bool draw_separator = false;
	};

}  // namespace bWidgets
