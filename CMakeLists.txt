cmake_minimum_required(VERSION 3.10)

project(main VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(main main.cpp)
add_subdirectory(BigNum)

target_link_libraries(main PUBLIC BigNum)
target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}") 

enable_testing()
include(GoogleTest)
add_executable(main_test test/test.cpp)
target_link_libraries(main_test BigNum gtest)

gtest_discover_tests(main_test)