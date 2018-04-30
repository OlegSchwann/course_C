cmake_minimum_required(VERSION 3.10)
project(swap_word C ASM) # подключаем .s как исходные коды.
set(CMAKE_C_STANDARD 11)
add_executable(swap_word main.c swap_word.s)
