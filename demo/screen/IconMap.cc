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

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#define MAGIC_ENUM_RANGE_MAX 1000
#include <magic_enum/magic_enum.hpp>

#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION
#define NANOSVG_ALL_COLOR_KEYWORDS
#include <nanosvg.h>
#include <nanosvgrast.h>

#include "png.h"

#include "File.h"

#include "IconMap.h"

namespace bWidgetsDemo
{

struct IconReadData
{
    png_structp png_ptr;
    png_infop info_ptr;
};

IconMapReader::IconMapReader() 
    : read_data(new IconReadData)
{
    read_data->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!read_data->png_ptr)
    {
        std::cout << "Error: Failed to create libPNG read_struct" << std::endl;
        return;
    }

    read_data->info_ptr = png_create_info_struct(read_data->png_ptr);
    if (!read_data->info_ptr)
    {
        png_destroy_read_struct(&read_data->png_ptr, nullptr, nullptr);
        std::cout << "Error: Failed to create libPNG info_struct" << std::endl;
        return;
    }
}

IconMapReader::~IconMapReader()
{
    png_destroy_info_struct(read_data->png_ptr, &read_data->info_ptr);
    png_destroy_read_struct(&read_data->png_ptr, nullptr, nullptr);
    read_data.reset();
}

static auto icons_check_if_file_is_png(File& file) -> bool
{
    const int num_header_bytes = 8;
    char header[num_header_bytes];

    if (file.readBytes(header, num_header_bytes, true))
    {
        return png_sig_cmp((png_const_bytep)header, 0, num_header_bytes) == 0;
    }

    return false;
}

static void libpng_copy_rows_into_pixmap(Pixmap& pixmap,
                                         const png_bytep* row_pointers,
                                         unsigned int position_x,
                                         unsigned int position_y)
{
    // Assumes pixmap channels and bit-depth match what row_pointers contains.
    const int height = pixmap.height();
    const int width = pixmap.width();
    const unsigned int bytes_per_pixel = (pixmap.getBitDepth() / 8) * pixmap.getNumChannels();

    for (int row = 0; row < height; row++)
    {
        std::copy_n(&row_pointers[position_y - row][position_x * bytes_per_pixel],
                    width * bytes_per_pixel,
                    &pixmap.getBytes()[row * width * bytes_per_pixel]);
    }
}

static void libpng_read_from_istream(png_structp png_ptr, png_bytep data, png_size_t length)
{
    File& file = *(File*)png_get_io_ptr(png_ptr);
    if (!file.readBytes((char*)data, (unsigned int)length, false))
    {
        std::cout << "Error reading PNG" << std::endl;
    }
}

auto IconMapReader::readIconMap(File& file) -> std::unique_ptr<IconMap>
{
    auto path = file.getPath();
    std::string extension = path.substr(path.find_last_of(".") + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), [](char c) { return (char)tolower(c); });

    if (extension == "png")
    {
        return readIconMapFromPNGFile(file);
    }
    
    return readIconMapFromSVGFiles(file);
}

auto IconMapReader::readIconMapFromPNGFile(File& file) -> std::unique_ptr<IconMap>
{
    if (!icons_check_if_file_is_png(file))
    {
        std::cout << "Error: File is not a valid PNG (" << file << ")" << std::endl;
        return nullptr;
    }

    png_set_read_fn(read_data->png_ptr, (png_voidp)&file, libpng_read_from_istream);

    png_read_png(read_data->png_ptr, read_data->info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    //	png_read_info(read_data.png_ptr, read_data.info_ptr);

    const png_uint_32 width = png_get_image_width(read_data->png_ptr, read_data->info_ptr);
    const png_uint_32 height = png_get_image_height(read_data->png_ptr, read_data->info_ptr);
    const png_byte bit_depth = png_get_bit_depth(read_data->png_ptr, read_data->info_ptr);
    const png_byte channels = png_get_channels(read_data->png_ptr, read_data->info_ptr);
    //	const png_byte color_type = png_get_color_type(read_data->png_ptr, read_data->info_ptr);

    png_bytep* row_pointers;
    row_pointers = (png_byte**)png_get_rows(read_data->png_ptr, read_data->info_ptr);

    assert((ICON_GRID_ROWS * (ICON_GRID_H + ICON_GRID_MARGIN) + (2 * ICON_GRID_MARGIN)) == height);
    // Icon image contains 46 pixels for category labels on the right side.
    assert((ICON_GRID_COLS * (ICON_GRID_W + ICON_GRID_MARGIN) + (2 * ICON_GRID_MARGIN)) ==
           width - 46);

    // TODO Icons don't scale with interface scale yet.
    std::unique_ptr<IconMap> icon_map(new IconMap);
    for (int y = 0, icon_index = 0; y < ICON_GRID_ROWS; y++)
    {
        for (int x = 0; x < ICON_GRID_COLS; x++)
        {
            std::unique_ptr<Icon> icon(new Icon(ICON_GRID_W, channels, bit_depth, icon_map->getPixelData(icon_index)));

            libpng_copy_rows_into_pixmap(
                icon->getPixmap(),
                row_pointers,
                x * (ICON_GRID_W + ICON_GRID_MARGIN) + ICON_GRID_MARGIN,
                height - (y * (ICON_GRID_H + ICON_GRID_MARGIN) + ICON_GRID_MARGIN));
            icon_map->icons.emplace_back(std::move(icon));

            ++icon_index;
        }
    }

    return icon_map;
}

auto IconMapReader::readIconMapFromSVGFiles(File& directory) -> std::unique_ptr<IconMap>
{
    std::unique_ptr<IconMap> iconMap(new IconMap);

    constexpr size_t prefixLen = sizeof("_icon") - 1;

    for (uint32_t i = 0; i < (uint32_t)BIFIconSvg::BIFSVGICONID_LAST; i++)
    {
        assert(i < MAGIC_ENUM_RANGE_MAX);

        auto name = std::string(magic_enum::enum_name(static_cast<BIFIconSvg>(i)));

        if (name.empty())
        {
            std::cout << "Warning: No name found for icon enum value: " << i << std::endl;
            continue;
        }

        name = name.substr(prefixLen);
        name += ".svg";
        
        std::transform(name.begin(), name.end(), name.begin(), [](char c) { return (char)tolower(c); });

        auto path = directory.getPath() + "/" + name;

        if (!std::filesystem::exists(path))
        {
            std::cout << "Warning: SVG icon file not found: " << path << std::endl;
            continue;
        }

        auto svgImage = nsvgParseFromFile(path.c_str(), "px", 96.0f);
        if (!svgImage)
        {
            std::cout << "Warning: Failed to parse SVG icon file: " << path << std::endl;
            continue;
        }
        
        auto rasterizer = nsvgCreateRasterizer();
        constexpr int size = std::max(ICON_GRID_W, ICON_GRID_H);
        float scale = svgImage->width ? (float)size / svgImage->width : 1.0f;

        std::unique_ptr<Icon> icon(new Icon(size, 
            IconMap::defaultNumChannel, 
            IconMap::defaultBitsPerChannel, 
            iconMap->getPixelData(i)));

        nsvgRasterize(rasterizer,
                      svgImage,
                      0.0f,
                      0.0f,
                      scale,
                      icon->getPixmap().getBytes(),
                      size,
                      size,
                      size * IconMap::defaultNumChannel);
        nsvgDeleteRasterizer(rasterizer);

        nsvgDelete(svgImage);

        iconMap->icons.emplace_back(std::move(icon));
    }

    return iconMap;
}

// --------------------------------------------------------------------

Icon::Icon(const unsigned int size,
           const unsigned int num_channels,
           const unsigned int bits_per_channel,
           unsigned char* pixelData)
    : _pixmap(size, size, num_channels, bits_per_channel, 0u, pixelData)
{
}

auto Icon::isValid() const -> bool
{
    return true;
}

auto Icon::getPixmap() -> Pixmap&
{
    return _pixmap;
}
auto Icon::getPixmap() const -> const Pixmap&
{
    return _pixmap;
}

IconMap::IconMap()
{
    icons.reserve(numIcons);
    iconPixelStorage.reset(new unsigned char[iconPixelStride * numIcons]());
}

auto IconMap::getIcon(unsigned int index) -> Icon&
{
    return *icons[index];
}

}  // namespace bWidgetsDemo
