cmake_minimum_required(VERSION 3.16)
project(HW1_1_Mul_Vec_)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS "-O0 -march=native")

add_executable(HW1_1_Mul_Vec_ main.cpp DVector.hpp Constants.hpp avx.cpp SSE.cpp raw.cpp)