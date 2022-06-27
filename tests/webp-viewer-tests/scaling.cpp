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
#include "../../webp-viewer/scaling.hpp"

TEST_CASE("scaling")
{
  SECTION("get_window_size")
  {
    const dimensions window { 640, 480 };

    SECTION("image is small enough to fit into window")
    {
      const dimensions img { 200, 123 };
      const auto scaled = get_window_size(img, window);

      REQUIRE( scaled.dims.width == 200 );
      REQUIRE( scaled.dims.height == 123 );
      REQUIRE( scaled.percentage == 100 );
    }

    SECTION("image too big: huge width")
    {
      const dimensions img { 1280, 124 };
      const auto scaled = get_window_size(img, window);

      REQUIRE( scaled.dims.width == 640 );
      REQUIRE( scaled.dims.height == 62 );
      REQUIRE( scaled.percentage == 50 );
    }

    SECTION("image too big: huge height")
    {
      const dimensions img { 200, 1920 };
      const auto scaled = get_window_size(img, window);

      REQUIRE( scaled.dims.width == 50 );
      REQUIRE( scaled.dims.height == 480 );
      REQUIRE( scaled.percentage == 25 );
    }
  }
}
