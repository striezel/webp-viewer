cmake_minimum_required (VERSION 3.8...3.31)

project(webp-viewer)

enable_testing()

# If the option ENABLE_LTO is enabled (e. g. via `cmake -DENABLE_LTO=ON`)
# during the build, then all binaries will use link-time optimization (LTO).
option(ENABLE_LTO "Enable link-time optimization" OFF)
# Not all compilers support LTO / IPO, so it has to be checked.
if (ENABLE_LTO)
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT HAS_LTO_SUPPORT OUTPUT LTO_FAIL_REASON
                      LANGUAGES C CXX)
  if (NOT HAS_LTO_SUPPORT)
    message(FATAL "IPO / LTO is not supported: ${LTO_FAIL_REASON}")
  else()
    message(STATUS "IPO / LTO is supported. Using it.")
  endif()
endif(ENABLE_LTO)

# If ENABLE_STATIC_STANDARD_LIBRARIES is on (e. g. via
# `cmake -DENABLE_STATIC_STANDARD_LIBRARIES=ON`), then the C++ standard
# libraries are linked statically. The option is off by default.
#
# Static linking increases the size of the binaries, but those binaries do not
# need the statically linked libraries to be present on the system.
option(ENABLE_STATIC_STANDARD_LIBRARIES "Link C++ standard libraries statically" OFF)
if (ENABLE_STATIC_STANDARD_LIBRARIES)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif (ENABLE_STATIC_STANDARD_LIBRARIES)

# Recurse into subdirectory for the main executable.
add_subdirectory (webp-viewer)

# Recurse into subdirectory for tests.
add_subdirectory (tests)
