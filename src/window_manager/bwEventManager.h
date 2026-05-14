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
    static bwEventManager& ensureEventManager();
    static void setupWindowHandlers(bwWindow& window);

    void waitEvents();
    bool processEvents(bwWindowManager::WindowList& windows);

    bool isClickEvent();

private:
    bwEventManager() = default;
    ~bwEventManager() = default;
    bwEventManager(bwEventManager const&) = delete;

    void operator=(bwEventManager const&) = delete;

    static void handleWindowResizeEvent(GLFWwindow* glfw_win, int32_t new_win_x, int32_t new_win_y);
    static void handleWindowContentScaleEvent(GLFWwindow* glfw_win,
                                              float new_scale_x,
                                              float new_scale_y);
    static void handleMouseMovementEvent(GLFWwindow* glfw_win, double x, double y);
    static void handleMouseButtonEvent(GLFWwindow* glfw_win,
                                       int32_t glfw_button,
                                       int32_t glfw_action,
                                       int32_t glfw_mods);
    static void handleMouseScrollEvent(GLFWwindow* glfw_win, double value_x, double value_y);
    static bwMouseButtonEvent::Button convertGlfwMouseButton(int32_t glfw_button);
    static bwMouseEvent::Type convertGlfwMouseButtonAction(int32_t glfw_action);
};

}  // namespace bWidgets
