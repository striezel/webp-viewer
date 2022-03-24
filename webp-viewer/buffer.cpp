/*
 -------------------------------------------------------------------------------
    This file is part of the WebP viewer.
    Copyright (C) 2022  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "buffer.hpp"
#include <cstring>

buffer::buffer()
: pointer(nullptr), data_size(0)
{
}

buffer::buffer(uint8_t * data_ptr, const size_t size)
: pointer(data_ptr), data_size(size)
{
}

buffer::buffer(const buffer& other)
: pointer(new uint8_t[other.data_size]), data_size(other.data_size)
{
  std::memcpy(pointer, other.pointer, other.data_size);
}

buffer::buffer(buffer&& other)
: pointer(other.pointer), data_size(other.data_size)
{
  other.pointer = nullptr;
  other.data_size = 0;
}

buffer::~buffer()
{
  delete[] pointer;
}

const uint8_t * buffer::data() const
{
  return pointer;
}

size_t buffer::size() const
{
  return data_size;
}
