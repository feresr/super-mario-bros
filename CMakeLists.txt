cmake_minimum_required(VERSION 3.16)

project(smb)

#for linux
#https://stackoverflow.com/questions/28395833/using-sdl2-with-cmake?answertab=votes#tab-top
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

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

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF})
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()
