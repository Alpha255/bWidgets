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

#include "bwWidget.h"

#include "window_manager/bwWindowManager.h"

#include "window_manager/bwEvent.h"

struct GLFWwindow;

namespace bWidgets
{

class bwEventManager
{
public:
    // Constructor is private (singleton!)
    static auto ensureEventManager() -> bwEventManager&;
    static void setupWindowHandlers(bwWindow& window);

    void waitEvents();
    auto processEvents(bwWindowManager::WindowList& windows) -> bool;

    auto isClickEvent() -> bool;

private:
    bwEventManager() = default;
    ~bwEventManager() = default;
    bwEventManager(bwEventManager const&) = delete;

    void operator=(bwEventManager const&) = delete;

    static void handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y);
    static void handleWindowContentScaleEvent(GLFWwindow* glfw_win,
                                              float new_scale_x,
                                              float new_scale_y);
    static void handleMouseMovementEvent(GLFWwindow* glfw_win, double x, double y);
    static void handleMouseButtonEvent(GLFWwindow* glfw_win,
                                       int glfw_button,
                                       int glfw_action,
                                       int glfw_mods);
    static void handleMouseScrollEvent(GLFWwindow* glfw_win, double value_x, double value_y);
    static auto convertGlfwMouseButton(int glfw_button) -> bwMouseButtonEvent::Button;
    static auto convertGlfwMouseButtonAction(int glfw_action) -> bwMouseEvent::Type;
};

}  // namespace bWidgets
