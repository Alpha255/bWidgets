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
#include "bwScrollView.h"
#include "bwStyleCSS.h"
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
#include "stylesheet/bwStyleSheet.h"
#include "window_manager/bwWindow.h"

#include "Stage.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo
{

std::unique_ptr<bwStyle> Stage::style = nullptr;
std::unique_ptr<bwStyleSheet> Stage::style_sheet = nullptr;
std::unique_ptr<bwFont> Stage::font = nullptr;
std::unique_ptr<bwIconMap> Stage::icon_map = nullptr;
float Stage::interface_scale = 1.0f;

bwScreenGraph::bwScreenGraph createScreenGraph(const uint32_t width, const uint32_t height)
{
    auto container = std::make_unique<bwScreenGraph::bwContainerNode>();
    container->createLayout<bwScrollViewLayout>()
        .setItemMargin(5u)
        .setPadding(7u);
    container->createWidget<bwScrollView>(*container, width, height);

    return bwScreenGraph::bwScreenGraph(std::move(container));
}

Stage::Stage(const uint32_t width, const uint32_t height)
    : screen_graph(createScreenGraph(width, height)), mask_width(width), mask_height(height)
{
    initFonts();
    initIcons();

    // After font-init!
    bwPainter::s_paint_engine = std::make_unique<GawainPaintEngine>(*font, *icon_map);
    bwStyleCSS::polish_cb = Stage::StyleSheetPolish;

    bwStyleManager& style_manager = bwStyleManager::getStyleManager();
    style_manager.registerDefaultStyleTypes();
    activateStyleID(bwStyle::TypeID::CLASSIC);

    setFontTightPositioning(true);
}

Stage::~Stage()
{
    GPUShader::clearCache();
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

void Stage::activateStyleID(bwStyle::TypeID type_id)
{
    style = std::unique_ptr<bwStyle>(bwStyleManager::createStyleFromTypeID(type_id));
    style->dpi_fac = interface_scale;
}

void Stage::draw()
{
    bwRectanglePixel stage_rect{ 0, int32_t(mask_width) - 1, 0, int32_t(mask_height - 1) };
    bwStyleProperties properties;
    bwColor clear_color{ 114u };

    if (style->type_id == bwStyle::TypeID::CLASSIC_CSS)
    {
        setStyleSheet(std::string(RESOURCES_PATH_STR) + "/" + "classic_style.css");
    }
    else if (style->type_id == bwStyle::TypeID::FLAT_LIGHT)
    {
        setStyleSheet(std::string(RESOURCES_PATH_STR) + "/" + "flat_light.css");
    }
    else if (style->type_id == bwStyle::TypeID::FLAT_DARK)
    {
        setStyleSheet(std::string(RESOURCES_PATH_STR) + "/" + "flat_dark.css");
    }
    else
    {
        style_sheet = nullptr;
    }

    bwStyleProperty& property = properties.addColor("background-color", clear_color);
    if (style_sheet)
    {
        style_sheet->resolveValue("Stage", bwWidget::State::NORMAL, property);
    }

    bwPainter::s_paint_engine->setupViewport(stage_rect, clear_color);

    resolveScreenGraphNodeLayout(screen_graph.Root(), stage_rect, interface_scale);

    bwScreenGraph::bwDrawer::draw(screen_graph, *style);
}

void Stage::StyleSheetPolish(bwWidget& widget)
{
    bwStyleSheet& stylesheet = *Stage::style_sheet;

    for (auto& property : widget.style_properties)
    {
        stylesheet.resolveValue(widget.getTypeIdentifier(), widget.getState(), *property);
    }
}

void Stage::setContentScale(const float scale_x, const float scale_y)
{
    auto& gwn_engine = dynamic_cast<GawainPaintEngine&>(*bwPainter::s_paint_engine);
    gwn_engine.m_scale_x = scale_x;
    gwn_engine.m_scale_y = scale_y;
    setFontSize(11.0f);
}

void Stage::setInterfaceScale(const float value)
{
    if (value != interface_scale)
    {
        interface_scale = value;
        style->dpi_fac = value;
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
    if (!style_sheet || (style_sheet->getFilepath() != filepath))
    {
        style_sheet = std::make_unique<bwStyleSheet>(filepath);
    }
    else
    {
        /* TODO skip if file didn't change. */
        style_sheet->reload();
    }
}

void Stage::handleMouseMovementEvent(const bwMouseEvent& event)
{
    const bwPoint& mouse_location = event.getMouseLocation();

    // TODO Multiple hovered items need to be possible (e.g. button + surrounding panel).

    screen_graph.event_dispatcher.dispatchMouseMovement(bwEvent(mouse_location));
}

void Stage::handleMouseButtonEvent(const bwMouseEvent& event)
{
    bwMouseButtonEvent bw_event(event.getButton(), event.getMouseLocation());
    bwEventDispatcher& dispatcher = screen_graph.event_dispatcher;

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
    bwMouseWheelEvent bw_event(dir, event.getMouseLocation());
    bwEventDispatcher& dispatcher = screen_graph.event_dispatcher;

    dispatcher.dispatchMouseWheelScroll(bw_event);
}

void Stage::handleWindowResizeEvent(const bwWindow& win)
{
    mask_width = win.getWidth();
    mask_height = win.getHeight();
}

}  // namespace bWidgetsDemo
