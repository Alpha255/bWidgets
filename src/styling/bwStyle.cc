#include <cassert>

#include "bwStyle.h"
#include "bwStyleManager.h"
#include "bwStyleProperties.h"

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
	uint32_t bwStyle::s_default_widget_size_hint = 20;

	bwStyle::bwStyle(TypeID type_id)
		: type_id(type_id)
	{
	}

	void bwStyle::polish(class bwWidget&)
	{
		/* Nothing by default. */
	}

	template<>
	void bwStyleHandle<bwAbstractButton>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwCheckbox>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwLabel>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwMenuBar>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwMenuButton>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwNumberSlider>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwPanel>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwPopupMenu>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwPushButton>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwRadioButton>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwScrollBar>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwScrollView>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

	template<>
	void bwStyleHandle<bwTextBox>::onRegisterStyleProperties(bwStyle::TypeID type_id, bwStyleProperties& properties)
	{
	}

}  // namespace bWidgets
