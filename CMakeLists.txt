cmake_minimum_required(VERSION 3.12.0)
project(prog2 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(prog2 graph.cpp main.cpp)
set_target_properties(prog2 PROPERTIES CXX_STANDARD_REQUIRED ON CXX_STANDARD 23)

