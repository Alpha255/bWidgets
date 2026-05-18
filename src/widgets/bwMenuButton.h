#pragma once

#include "bwWidget.h"
#include "bwAbstractButton.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets
{
	class bwMenuButton : public bwAbstractButton
	{
	public:
		bwMenuButton(const std::string& text)
			: bwAbstractButton(text)
		{
		}

		std::string_view getTypeIdentifier() const override;

		void draw(bwStyle& style) override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

	protected:
		inline bool isHovered() const
		{
			return getState() == State::HIGHLIGHTED;
		}
	};
}