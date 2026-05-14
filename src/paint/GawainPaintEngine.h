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

#include "paint/bwPaintEngine.h"

namespace bWidgets
{

class GawainPaintEngine : public bwPaintEngine
{
public:
    GawainPaintEngine(class bwFont&, class bwIconMap&);

    void setupViewport(const bwRectanglePixel&, const bwColor&) override;
    void enableMask(const bwRectanglePixel&) override;

    void drawPolygon(const class bwPainter&, const class bwPolygon&) override;
    void drawText(const class bwPainter&,
                  const std::string&,
                  const bwRectanglePixel&,
                  const TextAlignment) override;
    void drawIcon(const class bwPainter&,
                  const bwIconInterface&,
                  const bwRectanglePixel&,
                  const bwColor& color = bwColor::White) override;

    float m_scale_x{ 1.0f };
    float m_scale_y{ 1.0f };

private:
    bwFont& font;
    bwIconMap& icon_map;
};

}  // namespace bWidgets
