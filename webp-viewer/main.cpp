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
#include "glfw.hpp"
#include "scaling.hpp"
#include "webp.hpp"

const int rcInvalidParameter = 1;
const int rcGlfwError = 2;
const int rcInputOutputError = 3;
const int rcAnimationsNotSupported = 4;

void showVersion()
{
  std::cout << "webp-viewer, version 0.4.2, 2022-06-15\n"
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

  const auto buffer = read_file(file);
  if (!buffer.has_value())
  {
    std::cerr << "Error: Failed to read file!\n" << buffer.error() << std::endl;
    return rcInputOutputError;
  }

  const auto dims = get_dimensions(buffer.value());
  if (!dims.has_value())
  {
    std::cerr << "Error: " << file << " is not a WebP file!\n";
    return rcInputOutputError;
  }
  std::cout << "Image size: width: " << dims.value().width << ", height: "
            << dims.value().height << std::endl;

  const auto anims = has_animations(buffer.value());
  if (anims.has_value() && anims.value())
  {
    std::cerr << "Error: " << file << " contains animations, but the viewer "
              << "does not support loading WebP images with animations.\n";
    return rcAnimationsNotSupported;
  }

  const bool has_alpha = has_alpha_channel(buffer.value()).value_or(false);
  const colour_space cs = has_alpha ? colour_space::RGBA : colour_space::RGB;
  const auto data = get_image_data(buffer.value(), dims.value(), cs);
  if (!data.has_value())
  {
    std::cout << "Error: " << file << " could not be decoded as WebP file!\n";
    return rcInputOutputError;
  }

  if (!glfwInit())
  {
    std::cerr << "Initialization of GLFW failed!\n";
    return rcGlfwError;
  }

  const auto scaling = get_window_size(dims.value(), get_maximum_window_size());
  std::string title = "webp viewer";
  if (scaling.percentage < 100)
  {
    title += " (scaled: " + std::to_string(scaling.percentage) + " %)";
  }
  GLFWwindow * window = glfwCreateWindow(scaling.dims.width, scaling.dims.height,
                                         title.c_str(), nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    std::cerr << "Window creation failed!\n";
    return rcGlfwError;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  if (scaling.percentage == 100)
  {
    glfwSetWindowSizeLimits(window, scaling.dims.width, scaling.dims.height,
                            scaling.dims.width, scaling.dims.height);
  }

  std::cout << "OpenGL version:  " << glGetString(GL_VERSION) << "\n"
            << "OpenGL vendor:   " << glGetString(GL_VENDOR) << "\n"
            << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";

  GLuint textureName = 0;
  glGenTextures(1, &textureName);
  glBindTexture(GL_TEXTURE_2D, textureName);
  const GLint internal_format = has_alpha ? GL_RGBA8 : GL_RGB8;
  const GLenum format = has_alpha ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dims.value().width, dims.value().height,
               0, format, GL_UNSIGNED_BYTE, data.value().data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  std::cout << "Hint: Press ESC or Q to close the viewer window." << std::endl;

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName);
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
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteTextures(1, &textureName);
  glfwTerminate();
  return 0;
}
