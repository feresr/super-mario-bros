cmake_minimum_required(VERSION 3.16)

project(smb)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(smb src/Main.cpp src/Game.cpp src/ecs/ecs.cpp src/RenderSystem.cpp src/MapSystem.cpp include/Components.h)

set_property(TARGET smb PROPERTY CXX_STANDARD 17)
set_target_properties(smb PROPERTIES OUTPUT_NAME smb-${CMAKE_BUILD_TYPE})
target_include_directories(smb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(smb ${SDL2} ${SDL2_IMAGE})

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})