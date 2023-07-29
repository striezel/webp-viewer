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

#include <filesystem>
#include <iostream>
#include <string>
#include "glfw.hpp"
#include "gl_util.hpp"
#include "return_codes.hpp"
#include "viewer.hpp"
#include "webp.hpp"

void showVersion()
{
  std::cout << "webp-viewer, version 0.4.3, 2022-08-19\n"
            << "\n"
            << "Library versions:\n"
            << "  * libwebp: " << webp_version() << "\n"
            << "  * glfw:    " << glfw_version() << std::endl;
}

void showHelp()
{
  std::cout << "webp-viewer [OPTIONS] [FILE]\n"
            << "\n"
            << "Loads and shows a WebP image.\n"
            << "\n"
            << "options:\n"
            << "  -? | --help     - Shows this help message.\n"
            << "  -v | --version  - Shows version information.\n"
            << "  FILE            - Sets the file name of image to show.\n";
}

void key_callback(GLFWwindow* window, int key, [[maybe_unused]] int scancode,
                  int action, [[maybe_unused]] int mods)
{
   if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
     glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv)
{
  std::string file;
  if ((argc > 1) && (argv != nullptr))
  {
    for (int i = 1; i < argc; ++i)
    {
      if (argv[i] == nullptr)
      {
        std::cerr << "Error: Parameter at index " << i << " is null pointer!\n";
        return rcInvalidParameter;
      }
      const std::string param(argv[i]);
      if ((param == "-v") || (param == "--version"))
      {
        showVersion();
        return 0;
      }
      else if ((param == "-?") || (param == "/?") || (param == "--help"))
      {
        showHelp();
        return 0;
      }
      else
      {
        if (!file.empty())
        {
          std::cout << "Error: Please do not specify more than one file!\n";
          return rcInvalidParameter;
        }
        // Parameter might be a file.
        std::error_code error;
        if (!std::filesystem::exists(param, error) || error)
        {
          std::cerr << "Error: File " << param << " does not exist!\n";
          return rcInvalidParameter;
        }
        file = param;
      }
    }
  } // if arguments are there

  if (file.empty())
  {
    std::cout << "Error: No file has been specified.\n";
    return rcInvalidParameter;
  }

  if (!glfwInit())
  {
    std::cerr << "Initialization of GLFW failed!\n";
    return rcGlfwError;
  }

  const auto window_info = create_window_for_image(file);
  if (!window_info.has_value())
  {
    glfwTerminate();
    return window_info.error();
  }

  show_open_gl_info();

  glfwSetKeyCallback(window_info.value().window, key_callback);

  std::cout << "Hint: Press ESC or Q to close the viewer window." << std::endl;

  while (!glfwWindowShouldClose(window_info.value().window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, window_info.value().texture);
    glBegin(GL_QUADS);
      glColor3f(1.0, 1.0, 1.0);
      glTexCoord2f(0.0, 0.0);
      glVertex2d(-1.0, -1.0);
      glTexCoord2f(1.0, 0.0);
      glVertex2d(1.0, -1.0);
      glTexCoord2f(1.0, 1.0);
      glVertex2d(1.0, 1.0);
      glTexCoord2f(0.0, 1.0);
      glVertex2d(-1.0, 1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glfwSwapBuffers(window_info.value().window);
    glfwPollEvents();
  }
  glDeleteTextures(1, &window_info.value().texture);
  glfwTerminate();
  return 0;
}
