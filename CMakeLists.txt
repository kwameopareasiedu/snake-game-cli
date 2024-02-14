cmake_minimum_required(VERSION 3.27)
project(snake_game_cli C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snake_game_cli main.c snake.c food.c)
target_link_libraries(snake_game_cli ${CURSES_LIBRARIES})
