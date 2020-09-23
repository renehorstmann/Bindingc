cmake_minimum_required(VERSION 3.0)
project(py3_base)

set(CMAKE_C_STANDARD 11)

add_library(py3_base SHARED base_lib.c)
