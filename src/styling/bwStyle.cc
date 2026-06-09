#include "bwStyle.h"
#include "bwStyleManager.h"

#include "widgets/bwAbstractButton.h"
#include "widgets/bwCheckbox.h"
#include "widgets/bwLabel.h"
#include "widgets/bwMenuBar.h"
#include "widgets/bwMenuButton.h"
#include "widgets/bwNumberSlider.h"
#include "widgets/bwPanel.h"
#include "widgets/bwPopupMenu.h"
#include "widgets/bwPushButton.h"
#include "widgets/bwRadioButton.h"
#include "widgets/bwScrollBar.h"
#include "widgets/bwScrollView.h"
#include "widgets/bwTextBox.h"

namespace bWidgets
{

	template<class Widget>
	struct bwWidgetStyleRegister
	{
		bwWidgetStyleRegister()
		{
			bwStyleManager::get().registerWidgetStyle<Widget>();
		}
	};

#define WIDGET_STYLE_STATIC_REGISTER(Type) \
	static bwWidgetStyleRegister<Type> s_##Type##_style_register;

	WIDGET_STYLE_STATIC_REGISTER(bwAbstractButton);
	WIDGET_STYLE_STATIC_REGISTER(bwCheckbox);
	WIDGET_STYLE_STATIC_REGISTER(bwLabel);
	WIDGET_STYLE_STATIC_REGISTER(bwMenuBar);
	WIDGET_STYLE_STATIC_REGISTER(bwMenuButton);
	WIDGET_STYLE_STATIC_REGISTER(bwNumberSlider);
	WIDGET_STYLE_STATIC_REGISTER(bwPanel);
	WIDGET_STYLE_STATIC_REGISTER(bwPopupMenu);
	WIDGET_STYLE_STATIC_REGISTER(bwPushButton);
	WIDGET_STYLE_STATIC_REGISTER(bwRadioButton);
	WIDGET_STYLE_STATIC_REGISTER(bwScrollBar);
	WIDGET_STYLE_STATIC_REGISTER(bwScrollView);
	WIDGET_STYLE_STATIC_REGISTER(bwTextBox);

}  // namespace bWidgets
