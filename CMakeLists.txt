cmake_minimum_required(VERSION 3.10)
project("Tic-Tac-Toe")

set(CMAKE_CXX_STANDARD 11)
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS window system graphics REQUIRED)

add_executable("Tic-Tac-Toe" main.cpp)

target_link_libraries("Tic-Tac-Toe" sfml-main sfml-window sfml-system sfml-graphics)
