cmake_minimum_required(VERSION 3.18.2)
project(StringPrinter 
        LANGUAGES C CXX)

set(C_STANDARD 99)
set(CXX_STANDARD C14)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
add_subdirectory(Utils)
add_subdirectory(StringPrinter)