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

#include <cassert>
#include <string>

#include "bwUtil.h"

struct GLFWwindow;
struct Gwn_Context;

namespace bWidgets
{
	class bwPoint;
}

namespace bWidgetsDemo
{
	class Stage;
}

namespace bWidgets
{
	class bwWindow
	{
	public:
		bwWindow(const std::string& name, uint32_t size_x = 600, uint32_t size_y = 350);

		~bwWindow();

		enum WindowAction
		{
			WINDOW_ACTION_CONTINUE,
			WINDOW_ACTION_CLOSE,
		};
		WindowAction processEvents();
		void draw();

		bwPoint getCursorPosition() const;
		void handleResizeEvent(const int32_t new_win_x, const int32_t new_win_y);
		void handleContentScaleEvent(const float new_scale_x, const float new_scale_y);

		GLFWwindow& getGlfwWindow() const;

		int32_t getWidth() const;
		int32_t getHeight() const;

		bWidgetsDemo::Stage* getStage() const
		{
			return stage.get();
		}

		template<class T> void createStage()
		{
			stage = std::make_unique<T>(getWidth(), getHeight());
			setupStage();
		}

		friend bool operator==(const bwWindow& lhs, const bwWindow& rhs)
		{
			if (lhs.glfw_window == rhs.glfw_window)
			{
				assert(lhs.stage == rhs.stage);
				assert(lhs.width == rhs.width);
				assert(lhs.height == rhs.height);
				return true;
			}
			return false;
		}

	private:
		std::unique_ptr<bWidgetsDemo::Stage> stage;

		void setupStage();

		GLFWwindow* glfw_window;
		Gwn_Context* gwn_context;
		uint32_t VertexArrayID = 0;

		int32_t width, height;
	};

}  // namespace bWidgets
