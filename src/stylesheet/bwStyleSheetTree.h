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

#include <unordered_map>

#include "bwWidget.h"

namespace bWidgets
{

class bwStyleSheetTree
{
public:
    ~bwStyleSheetTree();

    bwStyleProperty& ensureNodeWithProperty(const std::string_view& class_name,
                                            const bwWidget::State state,
                                            const std::string_view& identifier,
                                            const bwStyleProperty::Type type);

    class bwStyleSheetNode& ensureNode(const std::string_view& class_name);

    const bwStyleProperty* resolveProperty(const std::string_view& class_name,
                                           const std::string_view& property_name,
                                           const bwWidget::State state);

private:
    class bwStyleSheetNode* lookupNode(const std::string_view& name) const;

    std::unordered_map<std::string, class bwStyleSheetNode*> nodes{ 0 };
};

}  // namespace bWidgets
