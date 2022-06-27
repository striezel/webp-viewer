/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for WebP viewer.
    Copyright (C) 2022  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

// This header tries to find the matching header for Catch (C++ Automated Test
// Cases in Headers).
#if defined(__has_include)
  #if __has_include(<catch2/catch.hpp>)
    #include <catch2/catch.hpp>
  #else
    #include <catch.hpp>
  #endif
#else
  // If there is no __has_include, just go with catch v1.
  #include <catch.hpp>
#endif
