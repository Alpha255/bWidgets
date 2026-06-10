#pragma once

#include "bwWidget.h"

namespace bWidgets
{
	class bwTextBox : public bwWidget
	{
		friend class bwTextBoxHandler;

	public:
		bwTextBox(std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		static constexpr std::string_view identifier = "bwTextBox";

		void draw() override;

		bwTextBox& setText(const std::string& value);
		const std::string_view getLabel() const override;

		bool canAlign() const override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		bwRectanglePixel selection_rectangle;

	protected:
		std::string text;
		bool is_text_editing = false;
	};

	class bwTextBoxHandler : public bwScreenGraph::bwEventHandler
	{
	public:
		bwTextBoxHandler(bwTextBox& textbox);
		virtual ~bwTextBoxHandler() = default;

		void onMouseEnter(bwEvent&) override;
		void onMouseLeave(bwEvent&) override;
		void onMousePress(bwMouseButtonEvent&) override;

	protected:
		void startTextEditing();
		void endTextEditing();

		bwTextBox& textbox;
		bool is_dragging = false;
	};

}  // namespace bWidgets
