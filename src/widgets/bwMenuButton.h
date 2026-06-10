#pragma once

#include "bwWidget.h"
#include "bwAbstractButton.h"
#include "bwPopupMenu.h"

namespace bWidgets
{
	class bwMenuButton : public bwAbstractButton
	{
	public:
		bwMenuButton(const std::string& text, bwMenuItem::onBuildPopupMenu&& onBuildFunc)
			: bwAbstractButton(text)
			, on_build_popupmenu(std::move(onBuildFunc))
		{
		}

		static constexpr std::string_view identifier = "bwMenuButton";

		void draw() override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

	protected:
		inline bool isHovered() const
		{
			return getState() == State::HIGHLIGHTED;
		}
	private:
		bwMenuItem::onBuildPopupMenu on_build_popupmenu;
	};
}