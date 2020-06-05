cmake_minimum_required(VERSION 3.15)
project(sp_network C)

set(CMAKE_C_STANDARD 11)

add_executable(sp_network main.c SPN.c SPN.h)