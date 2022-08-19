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

#ifndef WEBPVIEWER_DIMENSIONS_HPP
#define WEBPVIEWER_DIMENSIONS_HPP

#include <ostream>

struct dimensions
{
  int width;  /**< width in pixels */
  int height; /**< height in pixels */

  /** \brief Checks whether this instance is big enough to contain other dimensions.
   *
   * \param other  the other dimension to contain
   * \return Returns true, if this instance can contain the other. i. e. this
   *         is equal or greater in both width and height than other.
   *         Returns false otherwise.
   */
  bool can_contain(const dimensions& other) const
  {
    return (other.width <= width) && (other.height <= height);
  }
};

inline std::ostream& operator<<(std::ostream& s, const dimensions& dim)
{
  return s << dim.width << " px x " << dim.height << " px";
}

#endif // WEBPVIEWER_DIMENSIONS_HPP
