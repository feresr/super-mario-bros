project(engine)

find_package(SDL2_mixer REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB vendor ${CMAKE_CURRENT_SOURCE_DIR}/vendor/*.c ${CMAKE_CURRENT_SOURCE_DIR}/vendor/*.cpp)
add_library(engine STATIC ${src} ${vendor}) #HAD TO DECLARE main as a function for it to work

add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)


target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/)

file(COPY shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

##TODO FIX!: EXPOSING ALL LIBRARIES TO CLIENT [SHOULD BE PRIVATE]
target_link_libraries(engine glm spdlog ${SDL2_MIXER_LIBRARY} ${SDL2} ${SDL2_IMAGE} ${SDL2_TTF})