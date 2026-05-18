#pragma once

#include "screen/bwIconInterface.h"
#include "generics/bwPoint.h"
#include <stdint.h>
#include <assert.h>
#include <Windows.h>

namespace bWidgets
{
	enum class UserPreferenceFlags
	{
		AUTO_SAVE = (1 << 0),
		NUMINPUT_ADVANCED = (1 << 1),
		RECENT_SERACHES_DISABLE = (1 << 2),
		CLOSE_LEAVE = (1 << 3),
		UNUSED_4 = (1 << 4),
		TRACK_BALL = (1 << 5),
		UNUSED_6 = (1 << 6),
		UNUSED_7 = (1 << 7),
		MAT_ON_OB = (1 << 8),
		INTERNET_ALLOW = (1 << 9),
		DEVELOPER_UI = (1 << 10),
		TOOLTIPS = (1 << 11),
		TWO_BUTTON_MOUSE = (1 << 12),
		NO_NUMPAD = (1 << 13),
		ADD_CURSOR_ALIGNED = (1 << 14),
		FILE_COMPRESS = (1 << 15),
		UNUSED_16 = (1 << 16),
		CUSTOM_RANGE = (1 << 17),
		ADD_EDIT_MODE = (1 << 18),
		ADD_VIEW_ALIGNED = (1 << 19),
		REL_PATHS = (1 << 20),
		REL_EASE_CONFIRM = (1 << 21),
		SCRIPT_AUTOEXEC_DISABLE = (1 << 22),
		FILE_NO_UI = (1 << 23),
		NO_NEG_FRAMES = (1 << 24),
		TXT_TABS_SPACES_DISABLE = (1 << 25),
		TOOLTIP_PYTHON = (1 << 26),
		UNUSED_27 = (1 << 27),
		HIDE_DOT_DATABLOCK = (1 << 28),
	};

	struct dwTextIconPadFactor
	{
		float text;
		float icon;
		float icon_only;
	};

	struct bwUserPreferences
	{
		static bwUserPreferences& get()
		{
			static bwUserPreferences instance;
			return instance;
		}

		const float scale = 1.0f;
		const float default_dpi = 72u;
		const float text_margin = 0.4f;

		uint32_t line_width = 0;
		float dpi = 72u;
		float scale_factor = 1.0f;
		float inv_scale_factor = 1.0f; // wm_surface_constant_dpi_set_userpref, WM_window_dpi_set_userdef
		float pixel_size = 1.0f;
		uint32_t widget_unit = 20u;

		const dwTextIconPadFactor text_pad_default{ 1.5f, 0.25f, 0.0f };
		const dwTextIconPadFactor text_pad_compact{ 1.25f, 0.35f, 0.0f };
		const dwTextIconPadFactor text_pad_none{ 0.25f, 1.5f, 0.0f };

		inline float getDefaultIconSize() const
		{
			return bwIconInterface::ICON_DEFAULT_SIZE * scale_factor;
		}

		inline bwPoint getPanelMargin() const
		{
			return bwPoint(widget_unit * 0.4f, widget_unit * 0.1f);
		}

		inline float getPopupMargin() const
		{
			return scale_factor * 12.0f;
		}

		inline float getPopupMarginTop() const
		{
			return scale_factor * 10.0f;
		}

		inline float getMenuPadding() const
		{
			return widget_unit * 0.2f;
		}

		inline float getSubMenuPadding() const
		{
			return scale_factor * 6.0f;
		}

		inline float getMenuWidthMin() const
		{
			return widget_unit * 9.0f;
		}

		inline float getMenuScrollArrowSize() const
		{
			return scale_factor * 12.0f;
		}

		inline float getDefaultScreenDpi() const
		{
#if _WIN32
			return USER_DEFAULT_SCREEN_DPI;
#else
			assert(false);
			return 0.0f;
#endif
		}

		void initalizeWithDpiAwareness(const float dpiHint)
		{
			auto autoDpi = std::max<float>(dpiHint, getDefaultScreenDpi());
			this->dpi = autoDpi * scale * (default_dpi / getDefaultScreenDpi());
			this->pixel_size = static_cast<float>(std::max<uint32_t>(1u, std::max<uint32_t>(1u, static_cast<uint32_t>(dpi / 64u) + line_width)));
			this->scale_factor = dpi / default_dpi;
			this->inv_scale_factor = 1.0f / scale_factor;
			this->widget_unit = static_cast<uint32_t>(roundf(18.0f * scale_factor)) + (2.0f * pixel_size);
		}
	};

}  // namespace bWidgets