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

#include <cassert>
#include <cmath>
#include <iostream>

 // bWidgets lib
#include "paint/bwPainter.h"
#include "bwRange.h"
#include "bwRectangle.h"
#include "bwPopupMenu.h"
#include "bwScrollView.h"
#include "bwStyleManager.h"
#include "screen_graph/bwBuilder.h"
#include "screen_graph/bwDrawer.h"
#include "screen_graph/bwIterators.h"

#include "window_manager/bwEvent.h"
#include "utils/File.h"
#include "paint/gpu/GPUShader.h"
#include "paint/GawainPaintEngine.h"
#include "screen/bwIcon.h"
#include "screen/bwLayout.h"
#include "window_manager/bwWindow.h"
#include "styling/bwPreferences.h"

#include "Stage.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo
{
	std::unique_ptr<bwStyle> Stage::style = nullptr;
	std::unique_ptr<bwFont> Stage::font = nullptr;
	std::unique_ptr<bwIconMap> Stage::icon_map = nullptr;
	float Stage::interface_scale = 1.0f;

	std::unique_ptr<bwScreenGraph::bwScreenGraph> Stage::createScreenGraph(const uint32_t width, const uint32_t height)
	{
		auto container = std::make_unique<bwScreenGraph::bwContainerNode>();
		container->createLayout<bwScrollViewLayout>()
			.setItemMargin(5u)
			.setPadding(7u);
		container->createWidget<bwScrollView>(*container, width, height);

		return std::make_unique<bwScreenGraph::bwScreenGraph>(std::move(container));
	}

	Stage::Stage(const uint32_t width, const uint32_t height)
		: mask_width(width)
		, mask_height(height)
	{
		initFonts();
		initIcons();

		// After font-init!
		bwPainter::s_paint_engine = std::make_unique<GawainPaintEngine>(*font, *icon_map);

		setFontTightPositioning(true);
	}

	Stage::~Stage()
	{
		GPUShader::clearCache();
		screen_graph.reset();
	}

	void Stage::setup(const float scale_x, const float scale_y)
	{
		setContentScale(scale_x, scale_y);

		if (!screen_graph)
		{
			screen_graph = createScreenGraph(mask_width, mask_height);
			buildWidgets();
		}
	}

	void Stage::initFonts()
	{
		// Initialize freetype
		bwFont::initFontReading();

		// Initialize default font
		font = std::unique_ptr<bwFont>(bwFont::loadFont("Inter.ttf", RESOURCES_PATH_STR));
		font->setSize(11.0f * interface_scale);
	}

	void Stage::initIcons()
	{
		bwIconMapReader reader;

#if !USE_PNG_ICON_MAP
		File iconFile(RESOURCES_PATH_STR + std::string("/icons_svg"), std::ios::binary);
#else
		File iconFile(RESOURCES_PATH_STR + std::string("/blender_icons16.png"), std::ios::binary);
#endif

		icon_map = reader.readIconMap(iconFile);
	}

	void Stage::activateStyleID(bwStyle::Type type_id)
	{
	}

	void Stage::draw()
	{	
		assert(screen_graph);

		bwRectanglePixel stage_rect{ 0, int32_t(mask_width) - 1, 0, int32_t(mask_height - 1) };
		bwColor clear_color{ 114u };

		bwPainter::s_paint_engine->setupViewport(stage_rect, clear_color);

		screen_graph->Root().Layout()->resolve(screen_graph->Root(), stage_rect, interface_scale);

		bwScreenGraph::bwDrawer::draw(*screen_graph);

#if 0
		for (bwScreenGraph::bwNode& node : *screen_graph)
		{
			if (auto* menu = widget_cast<bwPopupMenu>(node.Widget()))
			{
				menu->drawDropdown(*style);
			}
		}
#endif
	}

	void Stage::StyleSheetPolish(bwWidget& widget)
	{
	}

	void Stage::setContentScale(const float scale_x, const float scale_y)
	{
		float dpiHint = bwUserPreferences::get().getDefaultScreenDpi() * scale_x;
		bwUserPreferences::get().initalizeWithDpiAwareness(dpiHint);

		auto& gwn_engine = dynamic_cast<GawainPaintEngine&>(*bwPainter::s_paint_engine);
		gwn_engine.m_scale_x = interface_scale;
		gwn_engine.m_scale_y = interface_scale;
		setFontSize(11.0f);
	}

	void Stage::setInterfaceScale(const float value)
	{
		if (value != interface_scale)
		{
			interface_scale = value;
			setFontSize(11.0f);
		}
	}

	void Stage::setFontSize(const float size)
	{
		auto& gwn_engine = dynamic_cast<GawainPaintEngine&>(*bwPainter::s_paint_engine);
		font->setSize(size * interface_scale * gwn_engine.m_scale_x);
	}

	void Stage::setFontAntiAliasingMode(const bwFont::AntiAliasingMode aa_mode)
	{
		font->setFontAntiAliasingMode(aa_mode);
	}

	void Stage::setFontTightPositioning(const bool value)
	{
		font->setTightPositioning(value);
	}

	void Stage::setFontHinting(const bool value)
	{
		font->setHinting(value);
	}

	void Stage::setFontSubPixelPositioning(const bool value)
	{
		font->setSubPixelPositioning(value);
	}

	void Stage::setStyleSheet(const std::string& filepath)
	{
	}

	void Stage::handleMouseMovementEvent(const bwMouseEvent& event)
	{
		assert(screen_graph);

		const bwPoint& mouse_location = event.getMouseLocation();

		// TODO Multiple hovered items need to be possible (e.g. button + surrounding panel).

		screen_graph->event_dispatcher.dispatchMouseMovement(bwEvent(mouse_location));
	}

	void Stage::handleMouseButtonEvent(const bwMouseEvent& event)
	{
		assert(screen_graph);

		bwMouseButtonEvent bw_event(event.getButton(), event.getMouseLocation());
		bwEventDispatcher& dispatcher = screen_graph->event_dispatcher;

		switch (event.getType())
		{
		case bwMouseEvent::Type::PRESS:
			dispatcher.dispatchMouseButtonPress(bw_event);
			break;
		case bwMouseEvent::Type::RELEASE:
			dispatcher.dispatchMouseButtonRelease(bw_event);
			break;
		default:
			break;
		}
	}

	void Stage::handleMouseScrollEvent(const bwMouseEvent& event, bwMouseWheelEvent::Direction dir)
	{
		assert(screen_graph);

		bwMouseWheelEvent bw_event(dir, event.getMouseLocation());
		bwEventDispatcher& dispatcher = screen_graph->event_dispatcher;

		dispatcher.dispatchMouseWheelScroll(bw_event);
	}

	void Stage::handleWindowResizeEvent(const bwWindow& win)
	{
		mask_width = win.getWidth();
		mask_height = win.getHeight();
	}

}  // namespace bWidgetsDemo
