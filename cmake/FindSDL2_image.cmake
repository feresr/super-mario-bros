# This module defines:
#
#   SDL2_IMAGE_LIBRARIES, the name of the library to link against
#   SDL2_IMAGE_INCLUDE_DIRS, where to find the headers
#   SDL2_IMAGE_FOUND, if false, do not try to link against

# Find path to the include (header) files #########
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATH_SUFFIXES SDL2 include/SDL2 include)
set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})

# Find path to the library (static) ###############

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
else()
    set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

IF(MSVC) # WIN32: True on windows systems, including win64.
    # Prefer static .lib in Windows (MSVC)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
ELSE(MSVC)
    # Prefer static .a in everything else (Linux, MacOS, MinGW)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
ENDIF(MSVC)

find_library(SDL2_IMAGE_LIBRARY  NAMES SDL2_image PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})

set(SDL2_IMAGE ${SDL2_IMAGE_LIBRARY})

include_directories(${SDL2_IMAGE_INCLUDE_DIRS})