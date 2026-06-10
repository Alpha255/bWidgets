/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#pragma once

#include <list>
#include <memory>

#include "event/bwEvent.h"
#include "bwStyle.h"
#include "screen_graph/bwNode.h"
#include "screen_graph/bwScreenGraph.h"

#include "rna/RNAProperty.h"

#include "screen/bwFont.h"
#include "screen/bwIcon.h"

namespace bWidgets
{
	class bwMouseEvent;
	class bwWindow;
}

namespace bWidgetsDemo
{
	class Stage
	{
		friend class UseFontSubPixelsToggleSetter;

	public:
		Stage(const uint32_t mask_width, const uint32_t mask_height);
		virtual ~Stage();

		void setup(const float scale_x, const float scale_y);

		void draw();

		void handleMouseMovementEvent(const bWidgets::bwMouseEvent& event);
		void handleMouseButtonEvent(const bWidgets::bwMouseEvent& event);
		void handleMouseScrollEvent(const bWidgets::bwMouseEvent& event, bWidgets::bwMouseWheelEvent::Direction dir);
		void handleWindowResizeEvent(const bWidgets::bwWindow& win);

		void setContentScale(float scale_x, float scale_y);
		static void setInterfaceScale(const float value);
		static void setFontSize(const float size);
		static void setFontTightPositioning(const bool value);
		static void setFontAntiAliasingMode(const bWidgets::bwFont::AntiAliasingMode aa_mode);
		static void setFontHinting(const bool value);
		static void setFontSubPixelPositioning(const bool value);
	
	protected:
		virtual std::unique_ptr<bWidgets::bwScreenGraph::bwScreenGraph> createScreenGraph(uint32_t width, uint32_t height);
		virtual void buildWidgets() {}

		virtual void activateStyleID(bWidgets::bwStyle::Type type_id);

		std::unique_ptr<bWidgets::bwScreenGraph::bwScreenGraph> screen_graph;

		// Static members, global UI data for all stages
		static std::unique_ptr<bWidgets::bwStyle> style;
		static std::unique_ptr<bWidgets::bwFont> font;
		static std::unique_ptr<bWidgets::bwIconMap> icon_map;
		static float interface_scale;

		uint32_t mask_width;
		uint32_t mask_height;

	private:
		static void StyleSheetPolish(bWidgets::bwWidget& widget);

		void initFonts();
		void initIcons();
		void setStyleSheet(const std::string& filepath);
	};

}  // namespace bWidgetsDemo
