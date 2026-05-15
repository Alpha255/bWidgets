#pragma once

#include <memory>
#include <string>

#include "bwUtil.h"
#include "bwWidget.h"
#include "bwWidgetBaseStyle.h"

namespace bWidgets
{
	class bwIconInterface;

	/**
	 * \brief Base class for button like widgets.
	 */
	class bwAbstractButton : public bwWidget
	{
	public:
		void draw(class bwStyle& style) override;
		void registerProperties() override;

		const std::string* getLabel() const override;
		bwAbstractButton& setLabel(const std::string& label);
		virtual const bwIconInterface* getIcon() const;

		std::unique_ptr<bwScreenGraph::bwEventHandler> createHandler() override;

		/**
		 * Function object called when applying changes to widget.
		 */
		std::unique_ptr<bwFunctorInterface> apply_functor{ nullptr };

		uint32_t rounded_corners;

	protected:
		// Protected constructor to force calling through inherited class (pseudo
		// abstract).
		bwAbstractButton(const std::string& text,
			std::optional<uint32_t> width_hint = std::nullopt,
			std::optional<uint32_t> height_hint = std::nullopt);
		void apply();

		std::string text;

	public:
		bwWidgetBaseStyle base_style;  // XXX public for setWidgetStyle. Should only
		// be temporarily needed.
	};

	class bwAbstractButtonHandler : public bwScreenGraph::bwEventHandler
	{
	public:
		bwAbstractButtonHandler(bwAbstractButton& button);
		virtual ~bwAbstractButtonHandler() = default;

		void onMouseEnter(bwEvent&) override;
		void onMouseLeave(bwEvent&) override;
		void onMousePress(bwMouseButtonEvent&) override;
		void onMouseRelease(bwMouseButtonEvent&) override;

	protected:
		bwAbstractButton& button;

		void apply();
	};

}  // namespace bWidgets
