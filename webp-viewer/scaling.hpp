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

#ifndef WEBPVIEWER_SCALING_HPP
#define WEBPVIEWER_SCALING_HPP

#include "dimensions.hpp"

/// Default size of the window, if size calculation fails.
const dimensions default_window_size = { 640, 480 };

/// Gets the maximum possible window size.
dimensions get_maximum_window_size();

/** \brief Gets the preferred windows size for display of an image.
 *
 * \param img   dimensions of the image
 * \param initial_window   initial size of the window
 * \return Returns the preferred window size.
 */
dimensions get_window_size(const dimensions& img, const dimensions& initial_window);

#endif // WEBPVIEWER_SCALING_HPP
