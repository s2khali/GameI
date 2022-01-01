cmake_minimum_required(VERSION 3.10)
project(gameI C)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(CMAKE_C_STANDARD 99)

add_executable(gameI WIN32 main.c)
