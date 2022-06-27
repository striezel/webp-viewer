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
#include "../../webp-viewer/dimensions.hpp"

TEST_CASE("dimensions")
{
  SECTION("can_contain")
  {
    dimensions dim1;
    dimensions dim2;

    SECTION("equal dimensions")
    {
      dim1.width = 640;
      dim1.height = 480;

      dim2.width = 640;
      dim2.height = 480;

      REQUIRE( dim1.can_contain(dim2) );
      REQUIRE( dim2.can_contain(dim1) );
    }

    SECTION("smaller dimensions")
    {
      dim1.width = 640;
      dim1.height = 480;

      dim2.width = 320;
      dim2.height = 460;

      REQUIRE( dim1.can_contain(dim2) );
      REQUIRE_FALSE( dim2.can_contain(dim1) );
    }

    SECTION("smaller width but larger height")
    {
      dim1.width = 640;
      dim1.height = 480;

      dim2.width = 320;
      dim2.height = 520;

      REQUIRE_FALSE( dim1.can_contain(dim2) );
      REQUIRE_FALSE( dim2.can_contain(dim1) );
    }

    SECTION("smaller height but larger width")
    {
      dim1.width = 640;
      dim1.height = 480;

      dim2.width = 800;
      dim2.height = 400;

      REQUIRE_FALSE( dim1.can_contain(dim2) );
      REQUIRE_FALSE( dim2.can_contain(dim1) );
    }
  }
}
