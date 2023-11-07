/*
 -------------------------------------------------------------------------------
    This file is part of the WebP viewer.
    Copyright (C) 2022, 2023  Dirk Stolle

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

#include "title.hpp"
#include <filesystem>

std::string generate_window_title(const std::string& file, const unsigned int scaling_percentage, const std::size_t current, const std::size_t total)
{
  std::string title;
  try
  {
    title = std::filesystem::path{file}.filename().string();
  }
  catch(...)
  {
    title = "webp viewer";
  }
  if (scaling_percentage != 100)
  {
    title += " (scaled: " + std::to_string(scaling_percentage) + " %)";
  }
  if (total > 1)
  {
    title += ", " + std::to_string(current + 1) + " / " + std::to_string(total);
  }
  return title;
}
