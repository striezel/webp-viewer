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

#include "image_data.hpp"

image_data::image_data(uint8_t * data_ptr, const size_t size_, const dimensions dims)
: data(data_ptr), data_size(size_), size(dims)
{
}

image_data::image_data(image_data&& op)
: data(op.data), data_size(op.data_size), size(op.size)
{
  op.data = nullptr;
  op.data_size = 0;
  op.size = { -1, -1 };
}

image_data::~image_data()
{
  delete [] data;
  data = nullptr;
}
