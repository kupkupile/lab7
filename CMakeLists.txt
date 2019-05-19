cmake_minimum_required(VERSION 3.12)
project(Task1 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES task1/task1a/task1a.c)            # Add main.cpp file of project root directory as source file
add_executable(taskFile ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable

add_library(Task1  task1/task1a/task1a.c)