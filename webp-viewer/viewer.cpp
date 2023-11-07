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

#include "viewer.hpp"
#include <filesystem>
#include <iostream>
#include "return_codes.hpp"
#include "scaling.hpp"
#include "title.hpp"
#include "webp.hpp"

std::string generate_window_title(const std::string& file, const scaling_data& scaling, const std::size_t current, const std::size_t total)
{
  return generate_window_title(file, scaling.percentage, current, total);
}

nonstd::expected<window_data, int> create_window_for_image(const std::string& file, const std::size_t current, const std::size_t total)
{
  const auto buffer = read_file(file);
  if (!buffer.has_value())
  {
    std::cerr << "Error: Failed to read file!\n" << buffer.error() << std::endl;
    return nonstd::make_unexpected(rcInputOutputError);
  }

  const auto dims = get_dimensions(buffer.value());
  if (!dims.has_value())
  {
    std::cerr << "Error: " << file << " is not a WebP file!\n";
    return nonstd::make_unexpected(rcInputOutputError);
  }
  std::cout << "Image size: " << dims.value() << std::endl;

  const auto anims = has_animations(buffer.value());
  const bool animated = anims.has_value() && anims.value();
  if (animated)
  {
    std::cerr << "Warning: " << file << " contains animations, but the viewer "
              << "only supports loading the first frame of WebP images with "
              << "animations.\n";
  }

  const bool has_alpha = has_alpha_channel(buffer.value()).value_or(false);
  const colour_space cs = has_alpha ? colour_space::RGBA : colour_space::RGB;
  const auto data = animated ? get_first_animation_frame(buffer.value(), dims.value(), cs)
                             : get_image_data(buffer.value(), dims.value(), cs);
  if (!data.has_value())
  {
    std::cout << "Error: " << file << " could not be decoded as WebP file!\n";
    return nonstd::make_unexpected(rcInputOutputError);
  }

  const auto scaling = get_window_size(dims.value(), get_maximum_window_size());
  const auto title = generate_window_title(file, scaling, current, total);
  GLFWwindow * window = glfwCreateWindow(scaling.dims.width, scaling.dims.height,
                                         title.c_str(), nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Window creation failed!\n";
    return nonstd::make_unexpected(rcGlfwError);
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowAspectRatio(window, scaling.dims.width, scaling.dims.height);
  // Note: Find a better solution to avoid "naked" new that does not involve
  // global or static data.
  title_data * ptr = new title_data;
  ptr->file = file;
  ptr->image_width = dims.value().width;
  ptr->current_index = current;
  ptr->total_files = total;
  glfwSetWindowUserPointer(window, static_cast<void*>(ptr));
  ptr = nullptr;

  GLuint textureName = 0;
  glGenTextures(1, &textureName);
  glBindTexture(GL_TEXTURE_2D, textureName);
  const GLint internal_format = has_alpha ? GL_RGBA8 : GL_RGB8;
  const GLenum format = has_alpha ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dims.value().width, dims.value().height,
               0, format, GL_UNSIGNED_BYTE, data.value().data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  window_data result { window, textureName };
  return result;
}
