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

#ifndef WEBPVIEWER_TITLE_HPP
#define WEBPVIEWER_TITLE_HPP

#include <filesystem>
#include <string>

/// Aux. data required for generation of a window title after window resize.
struct title_data
{
  std::filesystem::path file; /**< name of the image file */
  int image_width;            /**< actual, original width of the image */
  std::size_t current_index;  /**< current zero-based index of the file */
  std::size_t total_files;    /**< total number of files */
};

/** \brief Generates a title for the windows showing the image.
 *
 * \param file   file name of the image
 * \param scaling_percentage  percentage indicating the scaling factor (i. e. 100 for no scaling, 50 for half the original size)
 * \param current  zero-based index of the current image file
 * \param total    total number of images available
 * \return Returns a title suitable for the window.
 */
std::string generate_window_title(const std::filesystem::path& file, const unsigned int scaling_percentage, const std::size_t current, const std::size_t total);

#endif // WEBPVIEWER_TITLE_HPP
