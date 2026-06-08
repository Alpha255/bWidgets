#pragma once

#include <string>

#include "bwAbstractButton.h"

namespace bWidgets
{
	/**
	 * \brief A radio-button like widget retaining a pressed state once pressed.
	 *
	 * \note: The way this works right now is not that nice. It's however made so it
	 * can work nicely with Blender.
	 *
	 * Normally, there's a group of radio buttons, once one of them is pressed, the
	 * others get un-pressed. It's an exclusive behavior: There should only ever be
	 * one pressed radio button in the group. bWidgets doesn't ensure this behavior
	 * at all as of now. The host application needs to do this. In future, there
	 * should be a group-widget that ensures the exclusive behavior within bWidgets.
	 * We need to make Blender ready for it first, though (or make it optional).
	 */
	class bwRadioButton : public bwAbstractButton
	{
	public:
		bwRadioButton(const std::string& text,
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);

		static constexpr std::string_view identifier = "bwRadioButton";

		bool canAlign() const override;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;
	};

}  // namespace bWidgets
