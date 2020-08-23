project(engine)

find_package(SDL2_mixer REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(engine STATIC ${src})

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

##TODO FIX!: EXPOSING ALL LIBRARIES TO CLIENT [SHOULD BE PRIVATE]
target_link_libraries(engine PUBLIC ${SDL2_MIXER_LIBRARY} ${SDL2} ${SDL2_IMAGE} ${SDL2_TTF})