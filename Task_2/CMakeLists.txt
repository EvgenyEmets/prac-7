cmake_minimum_required(VERSION 3.12)
project(Task_2)
project(Gen)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(Task_2 main.cpp temperature.h solution.h mutation.h simulation.h csv_mod.h)
add_executable(Gen Generator.cpp solution.h csv_mod.h)