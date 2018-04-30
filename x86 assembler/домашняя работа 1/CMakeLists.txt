cmake_minimum_required(VERSION 3.10)
project(source C ASM)

set(CMAKE_C_STANDARD 11)

add_executable(source main.c count_e.s count_e.h)