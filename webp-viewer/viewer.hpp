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

#ifndef WEBPVIEWER_VIEWER_HPP
#define WEBPVIEWER_VIEWER_HPP

#include <string>
#include <GLFW/glfw3.h>
#include "../third-party/nonstd/expected.hpp"

struct window_data
{
  GLFWwindow* window; /**< pointer to the window */
  GLuint texture;     /**< associated OpenGL texture name */
};

/// Aux. data required for generation of a window title after window resize.
struct title_data
{
  std::string file;          /**< name of the image file */
  int image_width;           /**< actual, original width of the image */
  std::size_t current_index; /**< current zero-based index of the file */
  std::size_t total_files;   /**< total number of files */
};

/** \brief Generates a title for the windows showing the image.
 *
 * \param file   file name of the image
 * \param scaling_percentage  percentage indicating the scaling factor (i. e. 100 for no scaling, 50 for half the original size)
 * \param current  zero-based index of the current image file
 * \param total    total number of images available
 * \return Returns a title suitable for the window.
 */
std::string generate_window_title(const std::string& file, const unsigned int scaling_percentage, const std::size_t current, const std::size_t total);

/** \brief Creates a viewer window showing the given WebP image.
 *
 * \param file     path to the WebP image file
 * \param current  zero-based index of the current image file
 * \param total    total number of images available
 * \return Returns windows data containing a non-null pointer to a window and a
 *         non-zero OpenGL texture name in case of success.
 *         Returns an exit code, if an error occurred.
 */
nonstd::expected<window_data, int> create_window_for_image(const std::string& file, const std::size_t current, const std::size_t total);

#endif // WEBPVIEWER_VIEWER_HPP
