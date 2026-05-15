#pragma once

#include "bwAbstractButton.h"

namespace bWidgets
{
	class bwScrollBar : public bwAbstractButton
	{
	public:
		bwScrollBar(uint32_t width_hint = 0, uint32_t height_hint = 0);

		std::string_view getTypeIdentifier() const override;

		void draw(bwStyle& style) override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		float ratio = 0.0f;  // Ration between content and area height (max 1.0f).
		int32_t scroll_offset = 0;

	private:
		void setScrollOffset(int32_t value);
	};

}  // namespace bWidgets
