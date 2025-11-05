cmake_minimum_required(VERSION 4.0)
project(easy-bitmap-pixel-editor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -static")

add_executable(ebpe src/ebpe.cpp
        src/bmpFile.cpp
        src/bmpFile.h)
