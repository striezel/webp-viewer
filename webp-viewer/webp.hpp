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

#ifndef WEBPVIEWER_WEBP_HPP
#define WEBPVIEWER_WEBP_HPP

#include <filesystem>
#include <optional>
#include <string>
#include "../third-party/nonstd/expected.hpp"
#include "buffer.hpp"
#include "image_data.hpp"

/** \brief Gets the WebP library version.
 *
 * \return Returns a string containing the libwebp version, e.g. "1.2.3".
 */
std::string webp_version();

/** \brief Gets the width and height of image data.
 *
 * \param data    contents read from the WebP file
 * \return Returns the dimensions of the image, if it is valid.
 *         Returns an empty optional, if the data is invalid.
 */
std::optional<dimensions> get_dimensions(const buffer& data);

/** \brief Checks whether image data contains animations.
 *
 * \param data    contents read from the WebP file
 * \return Returns a bool indicating whether the file contains animations in
 *         case of success. Returns an empty optional, if the data is invalid.
 */
std::optional<bool> has_animations(const buffer& data);

/** \brief Checks whether image data contains an alpha channel.
 *
 * \param data    contents read from the WebP file
 * \return Returns a bool indicating whether the file contains an alpha channel
 *         in case of success. Returns an empty optional, if the data is invalid.
 */
std::optional<bool> has_alpha_channel(const buffer& data);

/** \brief Reads the whole file into a buffer.
 *
 * \param path   the path of the file
 * \return Returns a buffer containing a data pointer and size of the buffer in
 *         case of success. Returns an empty optional on failure.
 */
nonstd::expected<buffer, std::string> read_file(const std::filesystem::path& path);

/// possible colour spaces to use for WebP decoding
enum class colour_space { RGB, RGBA };

/** \brief Decodes raw data into RGB or RGBA data.
 *
 * \param data   contents read from the WebP file
 * \param dims   dimensions of the image
 * \param cs     the colour space for decoding, e. g. RGB or RGBA
 * \return Returns the decoded image data in case of success.
 *         Returns an empty optional on failure.
 */
std::optional<image_data> get_image_data(const buffer& data, const dimensions& dims, const colour_space cs);

/** \brief Decodes the first frame of raw data of an animated WebP file into RGB or RGBA data.
 *
 * \param data   contents read from the WebP file
 * \param dims   dimensions of the image
 * \param cs     the colour space for decoding, e. g. RGB or RGBA
 * \return Returns the decoded image data in case of success.
 *         Returns an empty optional on failure.
 */
std::optional<image_data> get_first_animation_frame(const buffer& data, const dimensions& dims, const colour_space cs);

#endif // WEBPVIEWER_WEBP_HPP
