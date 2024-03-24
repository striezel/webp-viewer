/*
 -------------------------------------------------------------------------------
    This file is part of the WebP viewer.
    Copyright (C) 2022, 2023, 2024  Dirk Stolle

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

#include <algorithm>
#include <cmath>
#if defined(_WIN32) || defined(_WIN64)
#include <cstdlib>
#endif
#include <filesystem>
#include <iostream>
#include <string>
#include "glfw.hpp"
#include "gl_util.hpp"
#include "return_codes.hpp"
#include "title.hpp"
#include "viewer.hpp"
#include "webp.hpp"

void showVersion()
{
  std::cout << "webp-viewer, version 0.6.3, 2024-03-24\n"
            << "\n"
            << "Library versions:\n"
            << "  * libwebp: " << webp_version() << "\n"
            << "  * glfw:    " << glfw_version() << std::endl;
}

void showHelp()
{
  std::cout << "webp-viewer [OPTIONS] [FILE]\n"
            << "\n"
            << "Loads and shows WebP images.\n"
            << "\n"
            << "options:\n"
            << "  -? | --help     - Shows this help message.\n"
            << "  -v | --version  - Shows version information.\n"
            << "  --shortcuts     - Shows available keyboard shortcuts.\n"
            << "  FILE            - Sets the file name of the image to show.\n"
            << "                    Can be repeated multiple times to view several images.\n";
}

void showKeys()
{
  std::cout << "webp-viewer supports the following keyboard shortcuts:\n"
            << "\n"
            << "ESC or Q                - Closes the viewer.\n"
            << "Left arrow key or A     - Shows the previous image.\n"
            << "Right arrow key or D    - Shows the next image.\n"
            << '\n'
            << "The previous and next image shortcuts only work when more than"
            << " one image was given as argument.\n";
}

std::vector<std::filesystem::path> files;
decltype(files)::size_type file_index = 0;
decltype(files)::size_type requested_file_index = 0;
window_data current_window { nullptr, 0 };
bool window_was_resized = false;

void move_to_next_image()
{
  requested_file_index = file_index + 1;
  if (requested_file_index >= files.size())
  {
    requested_file_index = 0;
  }
}

void move_to_previous_image()
{
  if (file_index == 0)
  {
    requested_file_index = files.size() - 1;
  }
  else
  {
    requested_file_index = file_index - 1;
  }
}

void key_callback(GLFWwindow* window, int key, [[maybe_unused]] int scancode,
                  int action, [[maybe_unused]] int mods)
{
   if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
     glfwSetWindowShouldClose(window, GLFW_TRUE);

  if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
     move_to_next_image();

  if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
     move_to_previous_image();
}

void resize_callback(GLFWwindow* window, int new_width,
                     [[maybe_unused]] int new_height)
{
  window_was_resized = true;
  void* user_ptr = glfwGetWindowUserPointer(window);
  if (user_ptr != nullptr)
  {
    const title_data* ptr = static_cast<title_data*>(user_ptr);
    const unsigned int percentage = std::max(1u, static_cast<unsigned int>(std::floor(static_cast<double>(new_width) / ptr->image_width * 100)));
    const auto title = generate_window_title(ptr->file, percentage,
                                             ptr->current_index, ptr->total_files);
    glfwSetWindowTitle(window, title.c_str());
  }
}

void close_callback(GLFWwindow* window)
{
  void* v_ptr = glfwGetWindowUserPointer(window);
  if (v_ptr != nullptr)
  {
    title_data* ptr = static_cast<title_data*>(v_ptr);
    delete ptr;
    glfwSetWindowUserPointer(window, nullptr);
  }
}

nonstd::expected<window_data, int> update_image()
{
  // Remove old callbacks.
  glfwSetKeyCallback(current_window.window, nullptr);
  glfwSetWindowSizeCallback(current_window.window, nullptr);
  close_callback(current_window.window);
  // Destroy old window.
  glfwDestroyWindow(current_window.window);
  // remove old texture name
  glDeleteTextures(1, &current_window.texture);
  const auto new_window = create_window_for_image(files[requested_file_index], requested_file_index, files.size());
  if (!new_window.has_value())
  {
    return nonstd::make_unexpected(new_window.error());
  }
  // Set callbacks on new window.
  glfwSetKeyCallback(new_window.value().window, key_callback);
  glfwSetWindowSizeCallback(new_window.value().window, resize_callback);
  glfwSetWindowCloseCallback(new_window.value().window, close_callback);
  // Update indices to avoid useless window re-creation.
  file_index = requested_file_index;
  return new_window.value();
}

int main(int argc, char* argv[])
{
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
      else if ((param == "--keys") || (param == "--shortcuts"))
      {
        showKeys();
        return 0;
      }
      else
      {
        // Parameter might be a file.
        std::filesystem::path file;
        try
        {
          #if !defined(_WIN32) && !defined(_WIN64)
          file = param;
          #else
          std::wstring utf16(param.size(), L'\0');
          const auto num_chars = std::mbstowcs(&utf16[0], param.c_str(), param.size());
          if (num_chars == static_cast<std::size_t>(-1))
          {
            std::cerr << "Error: " << param << " is not a valid parameter!\n";
            return rcInvalidParameter;
          }
          file = utf16;
          #endif
        }
        catch (...)
        {
          std::cerr << "Error: " << param << " is neither a valid parameter "
                    << "nor an existing WebP file!\n";
          return rcInvalidParameter;
        }
        std::error_code error;
        if (!std::filesystem::exists(file, error) || error)
        {
          std::cerr << "Error: File " << file << " does not exist!\n";
          return rcInvalidParameter;
        }
        files.push_back(file);
      }
    }
  } // if arguments are there

  if (files.empty())
  {
    std::cout << "Error: No file has been specified.\n";
    const auto example{std::filesystem::path("path") / "to" / "image.webp"};
    const std::string viewer = (argc > 0) && (argv[0] != nullptr) && (argv[0][0] != '\0')
                             ? argv[0] : "webp-viewer";
    std::cout << "Hint: Files to view can be specified as parameter, e.g.\n\n"
              << "    " << viewer << " " << example << "\n\nwould show the "
              << "WebP image " << example << ", if it existed.\n";
    return rcInvalidParameter;
  }

  file_index = 0;
  requested_file_index = 0;

  if (!glfwInit())
  {
    std::cerr << "Error: Initialization of GLFW failed!\n";
    return rcGlfwError;
  }

  const auto window_info = create_window_for_image(files[0], 0, files.size());
  if (!window_info.has_value())
  {
    glfwTerminate();
    return window_info.error();
  }
  current_window = window_info.value();

  show_open_gl_info();

  glfwSetKeyCallback(current_window.window, key_callback);
  glfwSetWindowSizeCallback(current_window.window, resize_callback);
  glfwSetWindowCloseCallback(current_window.window, close_callback);

  std::cout << "Hint: Press ESC or Q to close the viewer window." << std::endl;
  if (files.size() > 1)
  {
    std::cout << "Hint: Press right arrow key or D to show the next image.\n"
              << "Hint: Press left arrow key or A to show the previous image."
              << std::endl;
  }

  while (!glfwWindowShouldClose(current_window.window))
  {
    if (window_was_resized)
    {
      int width = 1;
      int height = 1;
      glfwGetWindowSize(current_window.window, &width, &height);
      glLoadIdentity();
      glViewport(0, 0, width, height);
      window_was_resized = false;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, current_window.texture);
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
    glfwSwapBuffers(current_window.window);
    glfwPollEvents();
    if (requested_file_index != file_index)
    {
      const auto new_window = update_image();
      if (!new_window.has_value())
      {
        glDeleteTextures(1, &current_window.texture);
        glfwTerminate();
        return new_window.error();
      }
      current_window = new_window.value();
    }
  }
  glDeleteTextures(1, &current_window.texture);
  close_callback(current_window.window);
  glfwTerminate();
  return 0;
}
