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

#include "bwDistance.h"
#include "event/bwEvent.h"
#include "bwWidget.h"

namespace bWidgets
{

class bwWindowEvent
{
    friend class bwEventManager;

protected:
    bwWindowEvent() = default;
};

class bwMouseEvent : bwWindowEvent
{
public:
    enum class Type
    {
        PRESS,
        RELEASE,
        MOVE,
        SCROLL_UP,
        SCROLL_DOWN,

        UNKNOWN
    };

    using Button = bwMouseButtonEvent::Button;

    bwMouseEvent(Type type, Button button, const bwPoint& location);

    bool isClick() const;

    Button getButton() const;
    Type getType() const;

    static const bwPoint& getMouseLocation();

private:
    Type type;
    Button button;

    static bwPoint location;
    // Location during previous mouse button press.
    static bwPoint last_down_location;
    static Button last_down_button;
};

}  // namespace bWidgets
