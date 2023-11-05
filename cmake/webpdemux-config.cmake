# - Try to find libwebpdemux
#
# Once done this will define
#  WEBPDEMUX_FOUND - System has libwebpdemux
#  WEBPDEMUX_INCLUDE_DIRS - The libwebpdemux include directories
#  WEBPDEMUX_LIBRARIES - The libraries needed to use libwebpdemux

find_path(WEBPDEMUX_INCLUDE_PATH webp/demux.h /usr/include/)
if (WEBPDEMUX_INCLUDE_PATH)
  set(WEBPDEMUX_INCLUDE_DIRS
    ${WEBPDEMUX_INCLUDE_PATH}
  )
else ()
  message("Include path for libwebpdemux could not be determined!")
endif (WEBPDEMUX_INCLUDE_PATH)

find_library(WEBPDEMUX_LIBRARIES webpdemux)

if (NOT WEBPDEMUX_LIBRARIES)
   message( "Library for webpdemux was not found!")
endif ()

if (WEBPDEMUX_INCLUDE_DIRS AND WEBPDEMUX_LIBRARIES)
   set(WEBPDEMUX_FOUND TRUE)
else ()
   set(WEBPDEMUX_FOUND FALSE)
endif ()

if (WEBPDEMUX_FIND_REQUIRED)
  if (NOT WEBPDEMUX_FOUND)
    message(FATAL_ERROR "Could not find libwebpdemux!")
  endif ()
endif ()

mark_as_advanced(WEBPDEMUX_LIBRARIES WEBPDEMUX_INCLUDE_DIRS)
