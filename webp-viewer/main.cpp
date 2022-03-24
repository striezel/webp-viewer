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

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "webp.hpp"

const int rcInvalidParameter = 1;
const int rcGlfwError = 2;

void showVersion()
{
  std::cout << "webp-viewer, version 0.1.1, 2022-03-24\n"
            << "\n"
            << "Library versions:\n"
            << "  * libwebp: " << webp_version() << std::endl;
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

  const auto buffer = read_file(file);
  if (!buffer.has_value())
  {
    std::cerr << "Error: Failed to read file!\n" << buffer.error() << std::endl;
    return 3;
  }

  const auto dims = get_dimensions(buffer.value().data(), buffer.value().size());
  if (!dims.has_value())
  {
    std::cout << "Error: " << file << " is not a WebP file!\n";
    return 3;
  }
  std::cout << "Image size: width: " << dims.value().width << ", height: "
            << dims.value().height << std::endl;

  if (!glfwInit())
  {
    std::cerr << "Initialization of GLFW failed!\n";
    return rcGlfwError;
  }

  GLFWwindow * window = glfwCreateWindow(640, 480, "webp viewer", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    std::cerr << "Window creation failed!\n";
    return rcGlfwError;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  std::cout << "OpenGL version:  " << glGetString(GL_VERSION) << "\n"
            << "OpenGL vendor:   " << glGetString(GL_VENDOR) << "\n"
            << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
      glColor3f(1.0f, 0, 0);
      glVertex2d(-0.5, -0.5);
      glColor3f(0, 1.0f, 0);
      glVertex2d(0.5, -0.5);
      glColor3f(0, 0, 1.0f);
      glVertex2d(0.0, 0.5);
    glEnd();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  std::cout << "It's the end of the program." << std::endl;
  return 0;
}
