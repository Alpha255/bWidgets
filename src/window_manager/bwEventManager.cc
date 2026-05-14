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

#include <list>

#include "bwRange.h"

#include "paint/gpu/GPU.h"
#include "Stage.h"

#include "window_manager/bwEventManager.h"

namespace bWidgets
{

bwEventManager& bwEventManager::ensureEventManager()
{
    static bwEventManager instance;
    return instance;
}

void bwEventManager::waitEvents()
{
    glfwWaitEvents();
}

bool bwEventManager::processEvents(bwWindowManager::WindowList& windows)
{
    for (bwWindow& win : windows)
    {
        /* TODO, should register handler lists that are handled in
         * event-manager, for now, just doing it with callbacks */
        if (win.processEvents() == bwWindow::WINDOW_ACTION_CLOSE)
        {
#if 0
			if (isMainWindow(win)) {
				return WM_ACTION_CLOSE;
			}
			else {
				removeWindow(win);
			}
#endif
            return false;
        }
    }

    return true;
}

void bwEventManager::setupWindowHandlers(bwWindow& window)
{
    GLFWwindow& glfw_window = window.getGlfwWindow();

    glfwSetWindowUserPointer(&glfw_window, &window);
    glfwSetWindowSizeCallback(&glfw_window, handleWindowResizeEvent);
    glfwSetWindowContentScaleCallback(&glfw_window, handleWindowContentScaleEvent);
    glfwSetCursorPosCallback(&glfw_window, handleMouseMovementEvent);
    glfwSetMouseButtonCallback(&glfw_window, handleMouseButtonEvent);
    glfwSetScrollCallback(&glfw_window, handleMouseScrollEvent);
}

void bwEventManager::handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y)
{
    auto* win = (bwWindow*)glfwGetWindowUserPointer(glfw_win);
    win->handleResizeEvent(new_win_x, new_win_y);
}

void bwEventManager::handleWindowContentScaleEvent(GLFWwindow* glfw_win,
                                                   float new_scale_x,
                                                   float new_scale_y)
{
    auto* win = (bwWindow*)glfwGetWindowUserPointer(glfw_win);
    win->handleContentScaleEvent(new_scale_x, new_scale_y);
}

bwMouseButtonEvent::Button bwEventManager::convertGlfwMouseButton(int glfw_button)
{
    switch (glfw_button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        return bwMouseButtonEvent::Button::LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return bwMouseButtonEvent::Button::RIGHT;
    }

    return bwMouseButtonEvent::Button::UNKNOWN;
}

bwMouseEvent::Type bwEventManager::convertGlfwMouseButtonAction(int glfw_action)
{
    switch (glfw_action)
    {
    case GLFW_PRESS:
        return bwMouseEvent::Type::PRESS;
    case GLFW_RELEASE:
        return bwMouseEvent::Type::RELEASE;
    }

    return bwMouseEvent::Type::UNKNOWN;
}

void bwEventManager::handleMouseMovementEvent(GLFWwindow* glfw_win, double /*x*/, double /*y*/)
{
    const bwWindow* win = (bwWindow*)glfwGetWindowUserPointer(glfw_win);
    const bwPoint& position = win->getCursorPosition();
    bwMouseEvent event(bwMouseEvent::Type::MOVE, bwMouseButtonEvent::Button::UNKNOWN, position);

    win->getStage()->handleMouseMovementEvent(event);
}

void bwEventManager::handleMouseButtonEvent(GLFWwindow* glfw_win,
                                            int glfw_button,
                                            int glfw_action,
                                            int /*glfw_mods*/)
{
    const bwWindow* win = (bwWindow*)glfwGetWindowUserPointer(glfw_win);
    const bwPoint& position = win->getCursorPosition();
    const bwMouseEvent::Type action_type = convertGlfwMouseButtonAction(glfw_action);
    const bwMouseButtonEvent::Button mouse_button = convertGlfwMouseButton(glfw_button);
    bwMouseEvent event(action_type, mouse_button, position);

    win->getStage()->handleMouseButtonEvent(event);
}

void bwEventManager::handleMouseScrollEvent(GLFWwindow* glfw_win,
                                            double /*value_x*/,
                                            double value_y)
{
    if ((value_y > -1) && (value_y < 1))
    {
        return;
    }

    const bwWindow* win = (bwWindow*)glfwGetWindowUserPointer(glfw_win);
    const bwMouseEvent::Type event_type = (value_y > 0) ? bwMouseEvent::Type::SCROLL_UP :
                                                          bwMouseEvent::Type::SCROLL_DOWN;
    const bwPoint& position = win->getCursorPosition();
    bwMouseEvent event(event_type, bwMouseButtonEvent::Button::WHEEL, position);
    bwMouseWheelEvent::Direction dir = (value_y > 0) ? bwMouseWheelEvent::Direction::UP :
                                                       bwMouseWheelEvent::Direction::DOWN;

    win->getStage()->handleMouseScrollEvent(event, dir);
}

}  // namespace bWidgets
