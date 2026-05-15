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

#include <iostream>

#include "paint/gpu/GPU.h"

#include "window_manager/bwEventManager.h"

#include "window_manager/bwWindowManager.h"

namespace bWidgets
{
	static void wm_glfw_error_callback(int32_t /* error */, const char* description)
	{
		std::cout << "Error: " << description << std::endl;
	}

	bwWindowManager::bwWindowManager() 
		: event_manager(bwEventManager::ensureEventManager())
		, main_win(nullptr)
	{
		if (!glfwInit())
		{
			assert(0);
		}
		glfwSetErrorCallback(wm_glfw_error_callback);

		//	GPU_init(); // needs context, so delay until window creation
	}

	bwWindowManager& bwWindowManager::getWindowManager()
	{
		static bwWindowManager instance;
		return instance;
	}

	bwWindowManager::~bwWindowManager()
	{
		windows.clear();  // Destroy windows before calling glfwTerminate().
		glfwTerminate();
	}

	bwWindowManager::WindowManagerAction bwWindowManager::processEvents()
	{
		event_manager.waitEvents();
		if (!event_manager.processEvents(windows))
		{
			return WM_ACTION_CLOSE;
		}

		return WM_ACTION_CONTINUE;
	}

	void bwWindowManager::drawWindows()
	{
		for (bwWindow& win : windows)
		{
			win.draw();
		}
	}

	void bwWindowManager::mainLoop()
	{
		while (processEvents() == WM_ACTION_CONTINUE)
		{
			drawWindows();
		}
	}

	bwWindow& bwWindowManager::addWindow(std::string name)
	{
		windows.emplace_back(name);
		if (windows.size() == 1)
		{
			main_win = &windows.back();
		}

		return windows.back();
	}

	void bwWindowManager::removeWindow(bwWindow& win)
	{
		windows.remove(win);
	}

	bool bwWindowManager::isMainWindow(const bwWindow& win) const
	{
		return &win == main_win;
	}

}  // namespace bWidgets
