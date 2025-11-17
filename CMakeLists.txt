cmake_minimum_required(VERSION 4.0)
project(easy-bitmap-pixel-editor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -static")

add_executable(ebpe 
        src/main.cpp
        src/bmpFile.cpp
        src/bmpFile.h
        src/cmdOpr.cpp
        src/cmdOpr.h
        src/helpCommand/helpCommand.h
        src/utils.cpp
        src/utils.h
)
