# webp-viewer - a very simple WebP image viewer

`webp-viewer` is a program that loads and displays a WebP image.

Animations are currently not supported, and probably never will be.

## Building from source

### Prerequisites

To build webp-viewer from source you need a C++ compiler with support for C++17,
CMake 3.8 or later, OpenGL, the GLFW library and the libwebp library.
pkg-config is required to make it easier to find compiler options for the
installed libraries.

It also helps to have Git, a distributed version control system, on your build
system to get the latest source code directly from the Git repository.

All of that can usually be installed be typing

    apt-get install cmake g++ git libgl-dev libglfw3-dev libwebp-dev pkg-config

or

    yum install cmake gcc-c++ git glfw-devel libwebp-devel pkgconfig

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

## Usage

```
webp-viewer [OPTIONS] [FILE]

Loads and shows a WebP image.

options:
  -? | --help     - Shows this help message.
  -v | --version  - Shows version information.
  FILE            - Sets the file name of image to show.
```

## History of changes

A changelog is provided as [separate file](./changelog.md).

## Copyright and Licensing

Copyright 2022  Dirk Stolle

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
