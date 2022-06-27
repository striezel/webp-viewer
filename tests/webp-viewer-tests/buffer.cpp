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
#include "../../webp-viewer/buffer.hpp"

TEST_CASE("buffer")
{
  SECTION("default constructor")
  {
    buffer buf;

    REQUIRE( buf.size() == 0 );
    REQUIRE( buf.data() == nullptr );
  }

  SECTION("constructor with existing buffer")
  {
    std::uint8_t * ptr = new std::uint8_t[2];
    ptr[0] = 42;
    ptr[1] = 21;

    buffer buf(ptr, 2);

    REQUIRE( buf.size() == 2 );
    REQUIRE( buf.data() == ptr );
    REQUIRE( buf.data()[0] == 42 );
    REQUIRE( buf.data()[1] == 21 );
  }

  SECTION("copy constructor")
  {
    std::uint8_t * ptr = new std::uint8_t[2];
    ptr[0] = 42;
    ptr[1] = 21;

    const buffer buf1(ptr, 2);

    buffer buf2(buf1);

    REQUIRE( buf2.size() == 2 );
    REQUIRE( buf2.data() != nullptr );
    REQUIRE( buf2.data()[0] == 42 );
    REQUIRE( buf2.data()[1] == 21 );

    // Pointers must be different.
    REQUIRE_FALSE( buf1.data() == buf2.data() );
  }

  SECTION("move constructor")
  {
    std::uint8_t * ptr = new std::uint8_t[2];
    ptr[0] = 42;
    ptr[1] = 21;

    buffer buf1(ptr, 2);
    buffer buf2(std::move(buf1));

    REQUIRE( buf2.size() == 2 );
    REQUIRE( buf2.data() == ptr );
    REQUIRE( buf2.data()[0] == 42 );
    REQUIRE( buf2.data()[1] == 21 );

    REQUIRE( buf1.size() == 0 );
    REQUIRE( buf1.data() == nullptr );
  }
}
