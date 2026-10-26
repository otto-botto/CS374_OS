cmake_minimum_required(VERSION 3.24)
project(search_files LANGUAGES C CXX)

# Allow the project to work with c and c++.
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

# Each module: a library, plus a test executable that links it.
add_library(movie "movie.c" "movie.h")
add_executable(movie_test "movie_test.cc")
target_link_libraries(movie_test GTest::gtest_main movie)

add_library(parser "parser.c" "parser.h")
target_link_libraries(parser PUBLIC movie)
add_executable(parser_test "parser_test.cc")
target_link_libraries(parser_test GTest::gtest_main parser)

add_library(collection "collection.c" "collection.h")
target_link_libraries(collection
        PUBLIC movie
        PRIVATE parser)
add_executable(collection_test "collection_test.cc")
target_link_libraries(collection_test GTest::gtest_main collection)

add_library(menu "menu.c" "menu.h")
target_link_libraries(menu PUBLIC collection)
add_executable(menu_test "menu_test.cc")
target_link_libraries(menu_test GTest::gtest_main menu)

add_library(search_menu "search_menu.c" "search_menu.h")
target_link_libraries(search_menu PUBLIC collection)
add_executable(search_menu_test "search_menu_test.cc")
target_link_libraries(search_menu_test GTest::gtest_main search_menu)

add_library(process_file "process_file.c" "process_file.h")
target_link_libraries(process_file PUBLIC collection)
add_executable(process_file_test "process_file_test.cc")
target_link_libraries(process_file_test GTest::gtest_main process_file)


# Main program.
add_executable(search_files main.c)
target_link_libraries(search_files parser collection menu search_menu process_file)
