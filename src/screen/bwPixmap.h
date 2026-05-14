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

#include <vector>

namespace bWidgets
{

class bwPixmap
{
public:
    bwPixmap(const int width,
             const int height,
             const unsigned int num_channels,
             const unsigned int bits_per_channel = 8,
             const unsigned int row_padding = 0,
             unsigned char* data = nullptr);

    ~bwPixmap();

    bwPixmap(const bwPixmap&) = default;
    bwPixmap& operator=(const bwPixmap&) = default;

    bwPixmap(bwPixmap&& other) noexcept
        : _bytes(other._bytes)
        , _width(other._width)
        , _height(other._height)
        , _num_channels(other._num_channels)
        , _bits_per_channel(other._bits_per_channel)
        , _row_padding(other._row_padding)
        , _num_bytes(other._num_bytes)
        , _owns_data(other._owns_data)
    {
        other._bytes = nullptr;
        other._owns_data = false;
    }
    bwPixmap& operator=(bwPixmap&& other) noexcept
    {
        _bytes = other._bytes;
        _width = other._width;
        _height = other._height;
        _num_channels = other._num_channels;
        _bits_per_channel = other._bits_per_channel;
        _row_padding = other._row_padding;
        _num_bytes = other._num_bytes;
        _owns_data = other._owns_data;

        other._bytes = nullptr;
        other._owns_data = false;
    }

    void fill(const unsigned char* bytes);

    unsigned char* getBytes() const
    {
        return _bytes;
    }

    int width() const;
    int height() const;
    unsigned int getBitDepth() const;
    unsigned int getNumChannels() const;
    uint32_t getNumBytes() const
    {
        return _num_bytes;
    }

private:
    unsigned char* _bytes;
    int _width, _height;
    unsigned int _num_channels;
    unsigned int _bits_per_channel;
    unsigned int _row_padding;
    uint32_t _num_bytes;
    bool _owns_data = false;
};

}  // namespace bWidgets
