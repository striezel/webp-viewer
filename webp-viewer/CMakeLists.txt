# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

project(webp-viewer)

set(webp_viewer_sources
    buffer.cpp
    dimensions.hpp
    glfw.cpp
    gl_util.cpp
    image_data.cpp
    scaling.cpp
    title.cpp
    viewer.cpp
    webp.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)

    if (CODE_COVERAGE)
        add_definitions (-O0)
    else()
        add_definitions (-O2)
    endif()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(webp-viewer ${webp_viewer_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(webp-viewer stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0")
    # If we are on Clang 7.x, then the filesystem library from GCC is better.
    target_link_libraries(webp-viewer stdc++fs)
  else ()
    # Use Clang's C++ filesystem library, it is recent enough.
    target_link_libraries(webp-viewer c++fs)
  endif ()
endif ()

# libGL
find_package (OpenGL REQUIRED COMPONENTS OpenGL)
if (OpenGL_OpenGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
  target_link_libraries (webp-viewer ${OPENGL_LIBRARIES})
else ()
  message ( FATAL_ERROR "libGL was not found!" )
endif ()

# GLFW for window handling
find_package (glfw3 REQUIRED)
if (glfw3_FOUND)
  target_link_libraries (webp-viewer glfw)
else ()
  message ( FATAL_ERROR "GLFW was not found!" )
endif ()

# libwebp
set(webp_DIR "../cmake/" )
find_package (webp REQUIRED)
if (WEBP_FOUND)
  include_directories(${WEBP_INCLUDE_DIRS})
  target_link_libraries (webp-viewer ${WEBP_LIBRARIES})
else ()
  message ( FATAL_ERROR "libwebp was not found!" )
endif ()

# libwebpdemux
set(webpdemux_DIR "../cmake/" )
find_package (webpdemux REQUIRED)
if (WEBPDEMUX_FOUND)
  include_directories(${WEBPDEMUX_INCLUDE_DIRS})
  target_link_libraries (webp-viewer ${WEBPDEMUX_LIBRARIES})
else ()
  message ( FATAL_ERROR "libwebpdemux was not found!" )
endif ()


# #################### #
# tests for executable #
# #################### #

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME webp-viewer_help
         COMMAND $<TARGET_FILE:webp-viewer> --help)

# short help parameter with question mark "-?"
add_test(NAME webp-viewer_help_question_mark
         COMMAND $<TARGET_FILE:webp-viewer> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
  add_test(NAME webp-viewer_help_question_mark_windows
           COMMAND $<TARGET_FILE:webp-viewer> /?)
endif ()

# parameter to show version number
add_test(NAME webp-viewer_version
         COMMAND $<TARGET_FILE:webp-viewer> --version)
