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

#include <array>
#include <iostream>
#include <memory>

#include "Pixmap.h"

#include "bwIconInterface.h"
#include "bwUtil.h"
#include "blender_icon_defines.h"

namespace bWidgetsDemo
{

struct IconReadData;
class File;

class Icon : public bWidgets::bwIconInterface
{
public:
    Icon(const unsigned int size,
         const unsigned int num_channels,
         const unsigned int bits_per_channel,
         unsigned char* pixelData = nullptr);

    auto isValid() const -> bool override;

    auto getPixmap() -> Pixmap&;  // TODO should get rid of this.
    auto getPixmap() const -> const Pixmap&;

private:
    Pixmap _pixmap;
};

class IconMap
{
    friend class IconMapReader;

public:
    ~IconMap() = default;

    auto getIcon(unsigned int index) -> Icon&;

    constexpr static uint32_t defaultNumChannel = 4u;
    constexpr static uint32_t defaultBitsPerChannel = 8u;
private:
    IconMap();

    auto getPixelData(uint32_t index) -> unsigned char* 
    {
        assert(index < numIcons);
        return &iconPixelStorage[index * iconPixelStride];
    }

    std::vector<std::unique_ptr<Icon>> icons;  // ICON_GRID_ROWS * ICON_GRID_COLS
    std::unique_ptr<unsigned char[]> iconPixelStorage;
    constexpr static size_t iconPixelStride = ICON_GRID_W * ICON_GRID_H *
                                              IconMap::defaultNumChannel *
                                              IconMap::defaultBitsPerChannel;
    constexpr static size_t numIcons = ICON_GRID_ROWS * ICON_GRID_COLS + 1u;
};

class IconMapReader
{
public:
    IconMapReader();
    ~IconMapReader();

public:
    auto readIconMap(File& file) -> std::unique_ptr<IconMap>;
protected:
    auto readIconMapFromPNGFile(File&) -> std::unique_ptr<IconMap>;
    auto readIconMapFromSVGFiles(File&) -> std::unique_ptr<IconMap>;

private:
    std::unique_ptr<IconReadData> read_data;
};

}  // namespace bWidgetsDemo
