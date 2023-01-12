cmake_minimum_required(VERSION 3.24)
project(M3D_project_M1)

set(CMAKE_CXX_STANDARD 17)

add_executable(project main.cpp tgaimage.cpp model.cpp model.h geometry.cpp geometry.h)
