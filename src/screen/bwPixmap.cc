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

#include "screen/bwPixmap.h"

namespace bWidgets
{

static uint32_t get_num_row_bytes_impl(int32_t width,
                                           uint32_t num_channels,
                                           uint32_t bits_per_channel,
                                           uint32_t row_padding)
{
    assert((bits_per_channel % 8) == 0);
    return width * (bits_per_channel / 8) * num_channels + row_padding;
}

bwPixmap::bwPixmap(const int32_t width,
                   const int32_t height,
                   const uint32_t num_channels,
                   const uint32_t bits_per_channel,
                   const uint32_t row_padding,
                   unsigned char* data)
    : _bytes(data),
      _width(width)
    , _height(height)
    , _num_channels(num_channels),
      _bits_per_channel(bits_per_channel)
    , _row_padding(row_padding)
    , _num_bytes(height * get_num_row_bytes_impl(width, num_channels, bits_per_channel, row_padding))
{
    if (!_bytes)
    {
        _bytes = new unsigned char[_num_bytes]();
        _owns_data = true;
    }
}

void bwPixmap::fill(const unsigned char* bytes)
{
    if (_bytes && _num_bytes > 0)
    {
        std::copy_n(bytes, _num_bytes, _bytes);
    }
}

int32_t bwPixmap::width() const
{
    return _width;
}
int32_t bwPixmap::height() const
{
    return _height;
}

uint32_t bwPixmap::getBitDepth() const
{
    return _bits_per_channel;
}

uint32_t bwPixmap::getNumChannels() const
{
    return _num_channels;
}

bwPixmap::~bwPixmap()
{
    if (_owns_data && _bytes)
    {
        delete[] _bytes;
        _bytes = nullptr;
    }
}

}  // namespace bWidgets
