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
#include <iostream>
#include "return_codes.hpp"
#include "scaling.hpp"
#include "webp.hpp"

nonstd::expected<window_data, int> create_window_for_image(const std::string& file)
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
  if (anims.has_value() && anims.value())
  {
    std::cerr << "Error: " << file << " contains animations, but the viewer "
              << "does not support loading WebP images with animations.\n";
    return nonstd::make_unexpected(rcAnimationsNotSupported);
  }

  const bool has_alpha = has_alpha_channel(buffer.value()).value_or(false);
  const colour_space cs = has_alpha ? colour_space::RGBA : colour_space::RGB;
  const auto data = get_image_data(buffer.value(), dims.value(), cs);
  if (!data.has_value())
  {
    std::cout << "Error: " << file << " could not be decoded as WebP file!\n";
    return nonstd::make_unexpected(rcInputOutputError);
  }

  const auto scaling = get_window_size(dims.value(), get_maximum_window_size());
  std::string title = "webp viewer";
  if (scaling.percentage < 100)
  {
    title += " (scaled: " + std::to_string(scaling.percentage) + " %)";
  }
  GLFWwindow * window = glfwCreateWindow(scaling.dims.width, scaling.dims.height,
                                         title.c_str(), nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Window creation failed!\n";
    return nonstd::make_unexpected(rcGlfwError);
  }

  glfwMakeContextCurrent(window);
  if (scaling.percentage == 100)
  {
    glfwSetWindowSizeLimits(window, scaling.dims.width, scaling.dims.height,
                            scaling.dims.width, scaling.dims.height);
  }

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
