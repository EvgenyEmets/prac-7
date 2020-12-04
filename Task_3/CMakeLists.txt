cmake_minimum_required(VERSION 3.12)
project(Task_3)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-lgtest")

add_executable(Task_3 library.cpp library.h main.cpp)