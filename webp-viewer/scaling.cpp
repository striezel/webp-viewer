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

#include "scaling.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>

dimensions get_maximum_window_size()
{
  GLFWmonitor * mon = glfwGetPrimaryMonitor();
  if (mon == nullptr)
    return default_window_size;

  int x_pos = 0;
  int y_pos = 0;
  int width = 0;
  int height = 0;
  glfwGetMonitorWorkarea(mon, &x_pos, &y_pos, &width, &height);
  if ((width <= 5) || (height <= 30))
    return default_window_size;

  return { width - 5, height - 30 };
}

scaling_data get_window_size(const dimensions& img, const dimensions& initial_window)
{
  if (initial_window.can_contain(img))
    return { img, 100 };

  const double w_ratio = static_cast<double>(img.width) / initial_window.width;
  const double h_ratio = static_cast<double>(img.height) / initial_window.height;
  const double max_ratio = w_ratio > h_ratio ? w_ratio : h_ratio;

  return {
           { static_cast<int>(std::ceil(img.width / max_ratio)),
             static_cast<int>(std::ceil(img.height / max_ratio)) },
           std::max(1u, static_cast<unsigned int>(std::floor(100.0 / max_ratio)))
         };
}
