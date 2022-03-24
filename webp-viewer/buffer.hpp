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

#ifndef WEBPVIEWER_BUFFER_HPP
#define WEBPVIEWER_BUFFER_HPP

#include <cstddef>
#include <cstdint>

struct buffer
{
  public:
    /// Constructs an empty buffer.
    buffer();

    /** \brief Takes ownership of an existing buffer.
     *
     * \param data_ptr   pointer to the existing buffer
     * \param size       size of the buffer in bytes
     */
    buffer(uint8_t * data_ptr, const size_t size);

    buffer(const buffer& other);
    buffer(buffer&& other);
    buffer operator=(const buffer& other) = delete;

    /// Destroys the buffer and frees the buffer memory.
    ~buffer();

    /** \brief Gets pointer to the internal data. It may be null.
     *
     * \return Returns the pointer to the buffer data.
     *         The pointer may be null, e. g. if the buffer size is zero.
     */
    const uint8_t * data() const;

    /** \brief Gets the size of the buffer in bytes.
     *
     * \return Returns the size of the buffer in bytes. This may be zero.
     */
    size_t size() const;
  private:
    uint8_t * pointer;
    size_t data_size;
};

#endif // WEBPVIEWER_BUFFER_HPP
