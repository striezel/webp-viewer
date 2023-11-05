/*
 -------------------------------------------------------------------------------
    This file is part of the WebP viewer.
    Copyright (C) 2022, 2023  Dirk Stolle

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
#include <webp/demux.h>

std::string webp_version()
{
  const int webp_version = WebPGetDecoderVersion();
  return std::to_string(webp_version >> 16).append(".")
      + std::to_string((webp_version & 0x0000FF00) >> 8).append(".")
      + std::to_string(webp_version % 256);
}

std::optional<dimensions> get_dimensions(const buffer& data)
{
  dimensions result { -1, -1 };
  const int code = WebPGetInfo(data.data(), data.size(), &result.width, &result.height);
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

std::optional<bool> has_animations(const buffer& data)
{
  WebPBitstreamFeatures features;
  features.has_animation = 0;
  if (WebPGetFeatures(data.data(), data.size(), &features) != VP8StatusCode::VP8_STATUS_OK)
    return std::nullopt;

  return features.has_animation != 0;
}

std::optional<bool> has_alpha_channel(const buffer& data)
{
  WebPBitstreamFeatures features;
  features.has_alpha = 0;
  if (WebPGetFeatures(data.data(), data.size(), &features) != VP8StatusCode::VP8_STATUS_OK)
    return std::nullopt;

  return features.has_alpha != 0;
}

std::optional<image_data> get_image_data(const buffer& data, const dimensions& dims, const colour_space cs)
{
  WebPDecoderConfig config;
  if (!WebPInitDecoderConfig(&config))
    return std::nullopt;

  // Flip image vertically, because OpenGL expects it that way.
  config.options.flip = 1;
  config.output.colorspace = (cs == colour_space::RGB) ? MODE_RGB : MODE_RGBA;
  // Set stride to multiple of four bytes, because OpenGL uses that internally
  // for its textures. If this is not done, then the image is skewed.
  const auto min_stride = dims.width * ((cs == colour_space::RGB) ? 3 : 4);
  const auto stride_mode = min_stride % 4;
  const size_t actual_stride = min_stride + (stride_mode != 0) * (4 - stride_mode);
  const size_t buffer_size = actual_stride * dims.height;
  image_data result { new uint8_t[buffer_size], buffer_size, dims };

  config.output.u.RGBA.stride = actual_stride;
  config.output.u.RGBA.size = buffer_size;
  config.output.u.RGBA.rgba = result.data;
  config.output.is_external_memory = 1;

  if (WebPDecode(data.data(), data.size(), &config) != VP8StatusCode::VP8_STATUS_OK)
    return std::nullopt;

  return result;
}

std::optional<image_data> get_first_animation_frame(const buffer& data, const dimensions& dims, const colour_space cs)
{
  WebPDecoderConfig config;
  if (!WebPInitDecoderConfig(&config))
    return std::nullopt;

  // Flip image vertically, because OpenGL expects it that way.
  config.options.flip = 1;
  config.output.colorspace = (cs == colour_space::RGB) ? MODE_RGB : MODE_RGBA;
  // Set stride to multiple of four bytes, because OpenGL uses that internally
  // for its textures. If this is not done, then the image is skewed.
  const auto min_stride = dims.width * ((cs == colour_space::RGB) ? 3 : 4);
  const auto stride_mode = min_stride % 4;
  const size_t actual_stride = min_stride + (stride_mode != 0) * (4 - stride_mode);
  const size_t buffer_size = actual_stride * dims.height;
  image_data result { new uint8_t[buffer_size], buffer_size, dims };

  config.output.u.RGBA.stride = actual_stride;
  config.output.u.RGBA.size = buffer_size;
  config.output.u.RGBA.rgba = result.data;
  config.output.is_external_memory = 1;

  WebPData api_data;
  api_data.bytes = data.data();
  api_data.size = data.size();

  WebPDemuxer* demuxer = WebPDemux(&api_data);
  if (demuxer == nullptr)
    return std::nullopt;

  WebPIterator iterator;
  if (!WebPDemuxGetFrame(demuxer, 1, &iterator))
  {
    WebPDemuxReleaseIterator(&iterator);
    WebPDemuxDelete(demuxer);
    return std::nullopt;
  }

  if (WebPDecode(iterator.fragment.bytes, iterator.fragment.size, &config) != VP8StatusCode::VP8_STATUS_OK)
  {
    WebPDemuxReleaseIterator(&iterator);
    WebPDemuxDelete(demuxer);
    return std::nullopt;
  }

  WebPDemuxReleaseIterator(&iterator);
  WebPDemuxDelete(demuxer);

  return result;
}
