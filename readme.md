# webp-viewer - a very simple WebP image viewer

[![GitHub CI Clang status](https://github.com/striezel/webp-viewer/workflows/Clang/badge.svg)](https://github.com/striezel/webp-viewer/actions)
[![GitHub CI GCC status](https://github.com/striezel/webp-viewer/workflows/GCC/badge.svg)](https://github.com/striezel/webp-viewer/actions)
[![GitHub CI MSYS2 status](https://github.com/striezel/webp-viewer/workflows/MSYS2/badge.svg)](https://github.com/striezel/webp-viewer/actions)
[![GitLab pipeline status](https://gitlab.com/striezel/webp-viewer/badges/main/pipeline.svg)](https://gitlab.com/striezel/webp-viewer/)

`webp-viewer` is a program that loads and displays a WebP image.

Animations are currently only partially supported, that is the viewer will only
load the first frame of an animated WebP image.

## Building from source

### Prerequisites

To build webp-viewer from source you need a C++ compiler with support for C++17,
CMake 3.8 or later, OpenGL, the GLFW library (version 3.3 or later) and the
libwebp library.
pkg-config is required to make it easier to find compiler options for the
installed libraries. Additionally, the program uses Catch (C++ Automated Test
Cases in Headers) to perform some tests.

It also helps to have Git, a distributed version control system, on your build
system to get the latest source code directly from the Git repository.

All of that can usually be installed be typing

    apt-get install catch cmake g++ git libgl-dev libglfw3-dev libwebp-dev pkg-config

or

    yum install catch-devel cmake gcc-c++ git glfw-devel libwebp-devel pkgconfig

into a root terminal.

### Getting the source code

Get the source directly from Git by cloning the Git repository and change to
the directory after the repository is completely cloned:

    git clone https://gitlab.com/striezel/webp-viewer.git
    cd webp-viewer

That's it, you should now have the current source code on your machine.

### Build process

The build process is relatively easy, because CMake does all the preparations.
Starting in the root directory of the source, you can do the following steps:

    mkdir build
    cd build
    cmake ../
    make -j4

If you want to run the test suite, too, then there is one more step:

    ctest -V

## Building from source on Windows

For information how to build webp-viewer on Windows, see the
[MSYS2 build instructions](./documentation/msys2-build.md).

## Usage

```
webp-viewer [OPTIONS] [FILE]

Loads and shows a WebP image.

options:
  -? | --help     - Shows this help message.
  -v | --version  - Shows version information.
  --shortcuts     - Shows available keyboard shortcuts.
  FILE            - Sets the file name of image to show.
                    Can be repeated multiple times to view several images.
```

## Keyboard shortcuts

The webp-viewer currently supports the following shortcuts:

| Key                               | Effect                    |
|-----------------------------------|---------------------------|
| <kbd>ESC</kbd> / <kbd>Q</kbd>     | Closes the viewer.        |
| <kbd>A</kbd> / <kbd>&#8592;</kbd> | Shows the previous image. |
| <kbd>D</kbd> / <kbd>&#8594;</kbd> | Shows the next image.     |

The previous and next image shortcuts only work when more than one image was
given as argument. (They would be useless for cases with only one image.)

## History of changes

A changelog is provided as [separate file](./changelog.md).

## Copyright and Licensing

Copyright 2022, 2023, 2024  Dirk Stolle

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
