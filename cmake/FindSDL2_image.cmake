# Find path to the include (header) files #########
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATH_SUFFIXES SDL2 include/SDL2 include)
set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})

# Find path to the library (static) ###############

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
else ()
    set(VC_LIB_PATH_SUFFIX lib/x86)
endif ()

# cmake respects the order of extensions specified on {CMAKE_FIND_LIBRARY_SUFFIXES} when looking for libraries
SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})

# SDL2_image intrinsic dependencies
find_library(PNGLIB png PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})
find_library(JPEG jpeg PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})
find_library(TIFF tiff PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})
find_library(WEBP webp PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})
find_library(ZLIB z PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})

set(SDL2_IMAGE ${PNGLIB} ${JPEG} ${TIFF} ${WEBP} ${ZLIB} ${SDL2_IMAGE_LIBRARY})

include_directories(${SDL2_IMAGE_INCLUDE_DIRS})