if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
else()
    set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

# find_package finds and runs /usr/local/Cellar/sdl2/2.0.12_1/lib/cmake/SDL2/sdl2-config.cmake
find_package(SDL2 PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})
set(SDL2 SDL2::SDL2-static)