cmake_minimum_required (VERSION 3.8)

# MSYS2 / MinGW uses Catch 3.x.
if (MINGW)
  set(HAS_CATCH_V3 ON)
else ()
  set(HAS_CATCH_V3 OFF)
endif ()

# Some newer distros like Fedora 38+ or Ubuntu 24.04+ also use Catch 3.x.
if (UNIX AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.22.0")
  cmake_host_system_information(RESULT DIST_NAME QUERY DISTRIB_NAME)
  cmake_host_system_information(RESULT DIST_VERSION_ID QUERY DISTRIB_VERSION_ID)

  if (DIST_NAME STREQUAL "Fedora Linux" AND DIST_VERSION_ID VERSION_GREATER_EQUAL "38")
    # Fedora 38 or later detected, set flag for Catch 3.x.
    set(HAS_CATCH_V3 ON)
  endif ()
  if (DIST_NAME STREQUAL "Ubuntu" AND DIST_VERSION_ID VERSION_GREATER_EQUAL "24.04")
    # Ubuntu 24.04 or later detected, set flag for Catch 3.x.
    set(HAS_CATCH_V3 ON)
  endif ()
endif()

add_subdirectory (webp-viewer-tests)
