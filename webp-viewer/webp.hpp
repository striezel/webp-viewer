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

#ifndef WEBPVIEWER_WEBP_HPP
#define WEBPVIEWER_WEBP_HPP

#include <optional>
#include <string>
#include "../third-party/nonstd/expected.hpp"
#include "buffer.hpp"

/** \brief Gets the WebP library version.
 *
 * \return Returns a string containing the libwebp version, e.g. "1.2.3".
 */
std::string webp_version();

struct dimensions
{
  int width;
  int height;
};

/** \brief Gets the width and height of image data.
 *
 * \param data    pointer to the image date
 * \param data_size   size of the image data
 * \return Returns the dimensions of the image, if it is valid.
 *         Returns an empty optional, if the data is invalid.
 */
std::optional<dimensions> get_dimensions(const uint8_t* data, size_t data_size);

/** \brief Reads the whole file into a buffer.
 *
 * \param path   the path of the file
 * \return Returns a buffer containing a data pointer and size of the buffer in
 *         case of success. Returns an empty optional on failure.
 */
nonstd::expected<buffer, std::string> read_file(const std::string& path);

struct image_data
{
  // TODO: Free data with WebPFree() after use.

  uint8_t * data;
  size_t data_size;
  dimensions size;
};

std::optional<image_data> get_rgb_data(const uint8_t* data, size_t data_size);

#endif // WEBPVIEWER_WEBP_HPP
