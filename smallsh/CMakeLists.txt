cmake_minimum_required(VERSION 3.24)
project(smallsh LANGUAGES C CXX)

# allow the project to work with C and C++
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set up GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

# each module: a library, a test executable, a link
add_library(parser "parser.c" "parser.h")
add_executable(parser_test "parser_test.cc")
target_link_libraries(parser_test GTest::gtest_main parser)

add_library(interpreter "interpreter.c" "interpreter.h")
target_link_libraries(interpreter PUBLIC parser)
add_executable(interpreter_test "interpreter_test.cc")
target_link_libraries(interpreter_test GTest::gtest_main interpreter)

# main program
add_executable(smallsh main.c)
target_link_libraries(smallsh parser interpreter)

gtest_discover_tests(parser_test)
gtest_discover_tests(interpreter_test)