cmake_minimum_required(VERSION 3.30)
project(sneki C)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/)

set(CMAKE_C_STANDARD 23)

add_executable(sneki main.c
        init.c
        init.h
        constants.h
        game.c
        game.h
        input.c
        input.h
        snake.c
        snake.h
        render.c
        render.h
        time.c
        time.h
        game_ball.c
        game_ball.h
        saves.c
        saves.h)

if(WIN32)
    set(SDL2_INCLUDE_DIR C:/msys64/mingw64/include/SDL2)
    set(SDL2_LIBRARY C:/msys64/mingw64/lib/libSDL2.dll.a)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(sneki ${SDL2_LIBRARY})
else()
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(sneki ${SDL2_LIBRARIES})
endif()