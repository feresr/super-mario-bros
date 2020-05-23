# Find path to the include (header) files #########
find_path(SDL2_TTF_INCLUDE_DIR SDL_TTF.h PATH_SUFFIXES SDL2 include/SDL2 include)
set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIR})

# Find path to the library (static) ###############

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else ()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif ()

# cmake respects the order of extensions specified on {CMAKE_FIND_LIBRARY_SUFFIXES} when looking for libraries
#SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/bzip2/bin/")


find_library(SDL2_TTF_LIBRARY NAMES SDL2_TTF PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})

# SDL2_TTF intrinsic dependencies
find_library(FREETYPE freetype PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})
find_library(BZIP bz2 PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})

set(SDL2_TTF ${FREETYPE} ${BZIP} ${SDL2_TTF_LIBRARY})

include_directories(${SDL2_TTF_INCLUDE_DIRS})