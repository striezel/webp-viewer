cmake_minimum_required (VERSION 3.8)

# test binary for component tests
project(webp-viewer-tests)

set(webp-viewer-tests_sources
    ../../webp-viewer/buffer.cpp
    ../../webp-viewer/dimensions.hpp
    ../../webp-viewer/glfw.cpp
    ../../webp-viewer/image_data.cpp
    ../../webp-viewer/scaling.cpp
    buffer.cpp
    detect_ci.cpp
    dimensions.cpp
    glfw.cpp
    image_data.cpp
    scaling.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -pedantic -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(webp-viewer-tests ${webp-viewer-tests_sources})

# GLFW
find_package (glfw3 REQUIRED)
if (glfw3_FOUND)
  target_link_libraries (webp-viewer-tests glfw)
else ()
  message ( FATAL_ERROR "GLFW was not found!" )
endif ()

# Link static library of Catch2 v3, if necessary.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(webp-viewer-tests Catch2::Catch2WithMain)
endif ()


# add test case
add_test(NAME webp-viewer-tests
         COMMAND $<TARGET_FILE:webp-viewer-tests>)
