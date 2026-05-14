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

#include <cassert>
#include <iostream>

#include "utils/File.h"

File::File(const std::string& path, std::ios::openmode mode)
    : _path(path)
    , _open_mode(mode)
{
}

std::string File::readIntoString()
{
    if (!_file_stream.is_open())
    {
        _file_stream.open(_path, _open_mode);
    }

    std::string string;
    std::string line;

    assert(_file_stream.is_open());
    while (getline(_file_stream, line))
    {
        string += line + '\n';
    }

    return string;
}

bool File::readBytes(char* bytes, const unsigned int num_bytes, bool reset_cursor)
{
    if (!_file_stream.is_open())
    {
        _file_stream.open(_path, _open_mode);
    }

    _file_stream.read(bytes, num_bytes);
    if (reset_cursor)
    {
        _file_stream.seekg(0);
    }
    return _file_stream.good();
}

std::ostream& operator<<(std::ostream& stream, const File& file)
{
    stream << file._path;
    return stream;
}
