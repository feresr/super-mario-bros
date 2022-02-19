cmake_minimum_required(VERSION 3.16)

project(smb)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if ( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" )
   set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 --preload-file ../assets@/assets --use-preload-plugins")
   set(CMAKE_EXECUTABLE_SUFFIX .html)
else ()
   find_package(SDL2_mixer REQUIRED)
   find_package(SDL2 REQUIRED)
   find_package(SDL2_image REQUIRED)
   find_package(SDL2_ttf REQUIRED)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(GLOB src
        "src/*.cpp"
        "src/systems/*.cpp"
        "src/scenes/*.cpp"
        "src/ecs/*.cpp")
file(GLOB imgui "vendor/imgui/*.cpp")
file(GLOB glad "vendor/glad/*.c")
add_executable(smb ${src} ${imgui} ${glad})

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set_property(TARGET smb PROPERTY CXX_STANDARD 17)
set_target_properties(smb PROPERTIES OUTPUT_NAME smb-${CMAKE_BUILD_TYPE})
target_include_directories(smb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(smb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
if ( NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" )
   target_link_libraries(smb PRIVATE ${SDL2_MIXER_LIBRARY} ${SDL2} ${SDL2_IMAGE} ${SDL2_TTF})
endif ()

