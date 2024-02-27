/*
 -------------------------------------------------------------------------------
    This file is part of the WebP viewer.
    Copyright (C) 2022, 2023, 2024  Dirk Stolle

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

#ifndef WEBPVIEWER_VIEWER_HPP
#define WEBPVIEWER_VIEWER_HPP

#include <filesystem>
#include <string>
#include <GLFW/glfw3.h>
#include "../third-party/nonstd/expected.hpp"

struct window_data
{
  GLFWwindow* window; /**< pointer to the window */
  GLuint texture;     /**< associated OpenGL texture name */
};

/** \brief Creates a viewer window showing the given WebP image.
 *
 * \param file     path to the WebP image file
 * \param current  zero-based index of the current image file
 * \param total    total number of images available
 * \return Returns windows data containing a non-null pointer to a window and a
 *         non-zero OpenGL texture name in case of success.
 *         Returns an exit code, if an error occurred.
 */
nonstd::expected<window_data, int> create_window_for_image(const std::filesystem::path& file, const std::size_t current, const std::size_t total);

#endif // WEBPVIEWER_VIEWER_HPP
