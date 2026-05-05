cmake_minimum_required(VERSION 3.22.1)
project(cellRobot CXX)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(lexGen)
add_subdirectory(syntaxGen)
add_subdirectory(prog)
add_subdirectory(visualiser)
