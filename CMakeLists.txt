cmake_minimum_required(VERSION 3.0)
project(Bindingc)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -march=native)  # march=native for best performance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function")


add_library(bindingc
        src/parsetypes.c
        src/parse.c
        src/parsefile.c
        src/types.c
        src/sort.c
        src/filter.c
        )

target_include_directories(bindingc PUBLIC include)


#
# Testing
#

# Enables ctest (after "make", test with "ctest" or "ctest -V")
enable_testing()

# Define the executable test
add_executable(main tests/main.c)
add_executable(testparse tests/parsetest.c)
add_executable(testfilter tests/filtertest.c)

# link to the library
target_link_libraries(main bindingc)
target_link_libraries(testparse bindingc)
target_link_libraries(testfilter bindingc)


# add the executable as test
add_test(TestParse testparse)
add_test(TestFilter testfilter)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})