#include "bwStyle.h"
#include "bwWidget.h"
#include "styling/bwPreferences.h"

namespace bWidgets
{
	bwWidget::bwWidget(std::optional<uint32_t> width_hint, std::optional<uint32_t> height_hint)
		: state(State::NORMAL)
		, rectangle(0, 0, 0, 0)
		, width_hint(width_hint.value_or(bwUserPreferences::get().widget_unit))
		, height_hint(height_hint.value_or(bwUserPreferences::get().widget_unit))
	{
	}

	bwWidget::State bwWidget::getState() const
	{
		return state;
	}

	bwWidget& bwWidget::setState(State value)
	{
		state = value;
		return *this;
	}

	bwWidget& bwWidget::hide(bool _hidden)
	{
		hidden = _hidden;
		return *this;
	}

	bool bwWidget::isHidden()
	{
		return hidden;
	}

	const std::string* bwWidget::getLabel() const
	{
		return nullptr;
	}

	bool bwWidget::canAlign() const
	{
		return false;
	}

	/**
	 * \brief Additional initialization that can't be done in #bwWidget constructor.
	 */
	void bwWidget::initialize()
	{
		/* Has to be done in init, can't be called from bwWidget constructor
		 * (wouldn't call overwritten function). 
		 */
	}

}  // namespace bWidgets
