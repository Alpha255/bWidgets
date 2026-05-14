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

#include <list>
#include <memory>
#include <optional>

#include "bwUtil.h"

#include "window_manager/bwWindow.h"

namespace bWidgets
{

class bwWindowManager
{
public:
    // Constructor is private (singleton!)
    static bwWindowManager& getWindowManager();
    ~bwWindowManager();

    void mainLoop();
    bwWindow& addWindow(std::string name);
    bool isMainWindow(const bwWindow& win) const;

    template<class T> bwWindow& addWindowWithStage(std::string name)
    {
        auto& win = windows.emplace_back(name);
        if (windows.size() == 1)
        {
            main_win = &windows.back();
        }

        win.createStage<T>();

        return windows.back();
    }

    void removeWindow(bwWindow& win);

    using WindowList = std::list<bwWindow>;

private:
    bwWindowManager();
    bwWindowManager(bwWindowManager const&) = delete;
    void operator=(bwWindowManager const&) = delete;

    enum WindowManagerAction
    {
        WM_ACTION_CONTINUE,
        WM_ACTION_CLOSE,
    };
    WindowManagerAction processEvents();
    void drawWindows();

    class bwEventManager& event_manager;
    WindowList windows;
    bwWindow* main_win;
};

}  // namespace bWidgets
