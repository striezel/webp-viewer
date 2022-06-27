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
#include <iostream>
#include <GLFW/glfw3.h>
#include "detect_ci.hpp"
#include "../../webp-viewer/glfw.hpp"

TEST_CASE("glfw")
{
  if (runsInCi())
  {
    std::clog << "Info: Test is run in an CI instance, so GLFW cannot be initialized." << std::endl;
  }
  else
  {
    SECTION("glfw_version")
    {
      REQUIRE( glfwInit() == GLFW_TRUE );

      const auto version = glfw_version();
      REQUIRE_FALSE( version.empty() );
      REQUIRE_FALSE( version.at(1) == '-' );

      glfwTerminate();
    }
  }
}
