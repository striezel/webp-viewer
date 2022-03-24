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

#include "webp.hpp"
#include <fstream>
#include <webp/decode.h>

std::string webp_version()
{
  const int webp_version = WebPGetDecoderVersion();
  return std::to_string(webp_version >> 16).append(".")
      + std::to_string((webp_version & 0x0000FF00) >> 8).append(".")
      + std::to_string(webp_version % 256);
}

std::optional<dimensions> get_dimensions(const uint8_t* data, size_t data_size)
{
  dimensions result { -1, -1 };
  const int code = WebPGetInfo(data, data_size, &result.width, &result.height);
  if (!code)
    return std::nullopt;

  return result;
}

nonstd::expected<buffer, std::string> read_file(const std::string& path)
{
  std::ifstream stream(path, std::ios::in | std::ios::binary);
  if (!stream.good())
  {
    return nonstd::make_unexpected("File could not be opened.");
  }

  stream.seekg(0, std::ios_base::end);
  const auto file_size = stream.tellg();
  if (file_size == std::ifstream::pos_type(-1))
  {
    return nonstd::make_unexpected("File size could not be determined.");
  }
  stream.seekg(0, std::ios_base::beg);
  if (!stream.good())
  {
    return nonstd::make_unexpected("File seek operation failed.");
  }

  uint8_t * data = new uint8_t[file_size];
  stream.read(reinterpret_cast<char*>(data), file_size);
  if (!stream.good())
  {
    delete[] data;
    return nonstd::make_unexpected("Not all data could be read from the file.");
  }

  stream.close();
  return buffer{ data, static_cast<size_t>(file_size) };
}

std::optional<image_data> get_rgb_data(const uint8_t* data, size_t data_size)
{
  image_data result { nullptr, 0, { -1, -1 } };
  uint8_t* pixels = WebPDecodeRGB(data, data_size,
                                        &result.size.width, &result.size.height);
  if (pixels == nullptr)
    return std::nullopt;
  result.data = pixels;
  result.data_size = 3 * result.size.width * result.size.height;
  return result;
}
