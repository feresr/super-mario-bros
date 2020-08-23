project(sandbox)

file(GLOB src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scenes/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/*.cpp)

add_executable(sandbox ${src})

target_link_libraries(sandbox PUBLIC engine)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_include_directories(sandbox PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/scenes
        ${CMAKE_CURRENT_SOURCE_DIR}/include/systems)