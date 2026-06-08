#include "bwMenuButton.h"
#include "event/bwEvent.h"

namespace bWidgets
{
	class bwMenuButtonEventHandler : public bwAbstractButtonHandler
	{
	public:
		using bwAbstractButtonHandler::bwAbstractButtonHandler;

		void onMousePress(bwMouseButtonEvent& event) override
		{
			if (event.button == bwMouseButtonEvent::Button::LEFT)
			{
				button.setState(bwWidget::State::SUNKEN);
				apply();
				event.swallow();
			}
		}
	};

	void bwMenuButton::draw(bwStyle& style)
	{
		bwAbstractButton::draw(style);
	}

	std::unique_ptr<bwScreenGraph::bwEventHandler> bwMenuButton::createHandler()
	{
		return std::make_unique<bwMenuButtonEventHandler>(*this);
	}
}
