cmake_minimum_required(VERSION 3.0)
project(Bindingc)

set(CMAKE_C_STANDARD 11)

add_library(bindingc src/parse.c)
target_include_directories(bindingc PUBLIC include)

add_executable(test tests/test.c)
target_link_libraries(test bindingc)
