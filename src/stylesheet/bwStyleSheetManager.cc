#include "bwStyleSheetManager.h"

namespace bWidgets
{
	bwStyleSheetManager& bwStyleSheetManager::get()
	{
		static bwStyleSheetManager sInstance;
		return sInstance;
	}
}