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

#include "Stage.h"

namespace bWidgetsDemo
{

class DefaultStage : public Stage
{
    friend class ScaleSetter;
    friend class StyleSetter;

public:
    DefaultStage(uint32_t mask_width, uint32_t mask_height);

private:
    void registerProperties(RNAProperties<DefaultStage>& properties);

    void activateStyleID(bWidgets::bwStyle::TypeID) override;
    void addStyleSelector(bWidgets::bwScreenGraph::bwLayoutNode& parent_node);
    void updateFontAAMode(bool value);

    void useStyleCSSVersionSet(const bool use_css_version);
    void updateStyleButtons();
    bool updateStyleButton(bWidgets::bwWidget& widget_iter);

    RNAProperties<DefaultStage> properties;

    const uint32_t padding = 10;
};

}  // namespace bWidgetsDemo
