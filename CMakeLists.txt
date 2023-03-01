cmake_minimum_required(VERSION 3.22)
project(M3D_project_M1)

set(CMAKE_CXX_STANDARD 20)

add_executable(project main.cpp tgaimage.h tgaimage.cpp model.h model.cpp draw.h draw.cpp vec.h matrix.h our_gl.cpp our_gl.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
