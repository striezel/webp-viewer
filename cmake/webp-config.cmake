# - Try to find libwebp
#
# Once done this will define
#  WEBP_FOUND - System has libwebp
#  WEBP_INCLUDE_DIRS - The libwebp include directories
#  WEBP_LIBRARIES - The libraries needed to use libwebp

find_path(WEBP_INCLUDE_PATH webp/decode.h /usr/include/)
if (WEBP_INCLUDE_PATH)
  set(WEBP_INCLUDE_DIRS
    ${WEBP_INCLUDE_PATH}
  )
else ()
  message("Include path for libwebp could not be determined!")
endif (WEBP_INCLUDE_PATH)

find_library(WEBP_LIBRARIES webp)

if (NOT WEBP_LIBRARIES)
   message( "Library for webp was not found!")
endif ()

if (WEBP_INCLUDE_DIRS AND WEBP_LIBRARIES)
   set(WEBP_FOUND TRUE)
else ()
   set(WEBP_FOUND FALSE)
endif ()

if (WEBP_FIND_REQUIRED)
  if (NOT WEBP_FOUND)
    message(FATAL_ERROR "Could not find libwebp!")
  endif ()
endif ()

mark_as_advanced(WEBP_LIBRARIES WEBP_INCLUDE_DIRS)
