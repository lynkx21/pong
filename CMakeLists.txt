cmake_minimum_required(VERSION 3.26)
project(pong)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS graphics window REQUIRED)

add_executable(pong main.cpp
        game.cpp
        ball.cpp
        paddle.cpp
        constants.cpp)
target_link_libraries(pong sfml-graphics sfml-window)
