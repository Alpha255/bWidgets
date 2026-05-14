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

#include "glm.hpp"

#include "window_manager/bwEventManager.h"

namespace bWidgets
{

bwPoint bwMouseEvent::location{};
bwPoint bwMouseEvent::last_down_location{};
bwMouseEvent::Button bwMouseEvent::last_down_button = Button::UNKNOWN;

bwMouseEvent::bwMouseEvent(Type type, Button _button, const bwPoint& _location) : type(type)
{
    if (type == Type::PRESS)
    {
        last_down_location = _location;
        last_down_button = _button;
    }
    else if (type == Type::MOVE)
    {
        _button = last_down_button;
    }
    location = _location;
    button = _button;
}

auto bwMouseEvent::isClick() const -> bool
{
    if ((type == Type::RELEASE) && (button == last_down_button))
    {
        const float dist = glm::distance(glm::vec2(location.x, location.y),
                                         glm::vec2(last_down_location.x, last_down_location.y));
        if (dist <= 3)
        {
            return true;
        }
    }

    return false;
}

auto bwMouseEvent::getButton() const -> Button
{
    return button;
}

auto bwMouseEvent::getType() const -> Type
{
    return type;
}

auto bwMouseEvent::getMouseLocation() -> const bwPoint&
{
    return location;
}

}  // namespace bWidgets
