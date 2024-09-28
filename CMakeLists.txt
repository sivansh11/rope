cmake_minimum_required(VERSION 3.10)

project(rope)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(rope rope.cpp main.cpp rope.hpp)
