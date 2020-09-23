cmake_minimum_required(VERSION 3.0)
project(Bindingc)

set(CMAKE_C_STANDARD 11)

#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -march=native)  # march=native for best performance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function -Wno-missing-braces")


add_library(bindingc
        src/log.c
        src/bindingtypes.c
        src/reflectionc.c
        src/py3/parsedtypes.c
        src/py3/generate.c
        )

target_include_directories(bindingc PUBLIC include)


#
# Testing
#

# Enables ctest (after "make", test with "ctest" or "ctest -V")
enable_testing()

# Define the executable test
add_executable(main tests/main.c)
add_executable(test_py3_base tests/test_py3_base.c)
add_executable(testgenerate tests/test_generate.c)

# link to the library
target_link_libraries(main bindingc)
target_link_libraries(test_py3_base bindingc)
target_link_libraries(testgenerate bindingc)


# link to Reflections
find_library(REFLECTIONC_LIB reflectionc)
target_link_libraries(main ${REFLECTIONC_LIB})
target_link_libraries(test_py3_base ${REFLECTIONC_LIB})
target_link_libraries(testgenerate ${REFLECTIONC_LIB})


# add the executable as test
add_test(TestGenerate testgenerate)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})