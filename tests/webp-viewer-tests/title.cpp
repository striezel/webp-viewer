/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for WebP viewer.
    Copyright (C) 2023  Dirk Stolle

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
#include <filesystem>
#include "../../webp-viewer/title.hpp"

TEST_CASE("window title generation")
{
  SECTION("generate_window_title")
  {
    SECTION("single image, not scaled")
    {
      const auto path = (std::filesystem::path("foo") / "bar.webp").string();
      REQUIRE( generate_window_title(path, 100, 0, 1) == "bar.webp" );
    }

    SECTION("single image, scaled")
    {
      const auto path = (std::filesystem::path("foo") / "blob_pic.webp").string();
      REQUIRE( generate_window_title(path, 123, 0, 1) == "blob_pic.webp (scaled: 123 %)" );
      REQUIRE( generate_window_title(path, 75, 0, 1) == "blob_pic.webp (scaled: 75 %)" );
    }

    SECTION("multiple images, not scaled")
    {
      const auto path = (std::filesystem::path("foo") / "blah" / "some thing.webp").string();
      REQUIRE( generate_window_title(path, 100, 0, 2) == "some thing.webp, 1 / 2" );
      REQUIRE( generate_window_title(path, 100, 41, 451) == "some thing.webp, 42 / 451" );
      REQUIRE( generate_window_title(path, 100, 450, 451) == "some thing.webp, 451 / 451" );
    }

    SECTION("multiple images, scaled")
    {
      const auto larger_path = (std::filesystem::path("foo") / "larger.webp").string();
      REQUIRE( generate_window_title(larger_path, 123, 0, 2) == "larger.webp (scaled: 123 %), 1 / 2" );
      REQUIRE( generate_window_title(larger_path, 234, 41, 451) == "larger.webp (scaled: 234 %), 42 / 451" );
      REQUIRE( generate_window_title(larger_path, 111, 450, 451) == "larger.webp (scaled: 111 %), 451 / 451" );

      const auto smaller_path = (std::filesystem::path("foo") / "smaller.webp").string();
      REQUIRE( generate_window_title(smaller_path, 99, 0, 2) == "smaller.webp (scaled: 99 %), 1 / 2" );
      REQUIRE( generate_window_title(smaller_path, 12, 41, 451) == "smaller.webp (scaled: 12 %), 42 / 451" );
      REQUIRE( generate_window_title(smaller_path, 8, 450, 451) == "smaller.webp (scaled: 8 %), 451 / 451" );
    }
  }
}
