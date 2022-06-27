/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for WebP viewer.
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

#include "locate_catch.hpp"
#include "../../webp-viewer/image_data.hpp"

TEST_CASE("image_data")
{
  SECTION("constructor with existing buffer")
  {
    std::uint8_t * ptr = new std::uint8_t[2];
    ptr[0] = 42;
    ptr[1] = 21;

    image_data data(ptr, 2, dimensions{ 123, 456 });

    REQUIRE( data.data == ptr );
    REQUIRE( data.data_size == 2 );
    REQUIRE( data.size.width == 123 );
    REQUIRE( data.size.height == 456 );
  }

  SECTION("move constructor")
  {
    std::uint8_t * ptr = new std::uint8_t[2];
    ptr[0] = 42;
    ptr[1] = 21;

    image_data original_data(ptr, 2, dimensions{ 123, 456 });
    image_data moved_data(std::move(original_data));

    REQUIRE( moved_data.data == ptr );
    REQUIRE( moved_data.data_size == 2 );
    REQUIRE( moved_data.size.width == 123 );
    REQUIRE( moved_data.size.height == 456 );

    REQUIRE( original_data.data == nullptr );
    REQUIRE( original_data.data_size == 0 );
    REQUIRE( original_data.size.width == -1 );
    REQUIRE( original_data.size.height == -1 );
  }
}
