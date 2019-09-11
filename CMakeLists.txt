cmake_minimum_required(VERSION 3.0)
project(Bindingc)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -march=native)  # march=native for best performance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function")


add_library(bindingc src/parse.c)
target_include_directories(bindingc PUBLIC include)


#
# Testing
#

# Enables ctest (after "make", test with "ctest" or "ctest -V")
enable_testing()

# Define the executable test
add_executable(main tests/main.c)
add_executable(testparse tests/parsetest.c)

# link to the library
target_link_libraries(main bindingc)
target_link_libraries(testparse bindingc)


# add the executable as test
add_test(TestParse testparse)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})