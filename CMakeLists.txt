cmake_minimum_required(VERSION 3.10)

project(rope)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(rope main.cpp rope.hpp)

target_compile_options(rope PUBLIC -fsanitize=address)
target_link_options(rope PUBLIC -fsanitize=address)
target_compile_options(rope PUBLIC -fsanitize=undefined)
target_link_options(rope PUBLIC -fsanitize=undefined)
