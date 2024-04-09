cmake_minimum_required(VERSION 3.27)
project(ZondaV2 C)

set(CMAKE_C_STANDARD 11)

add_executable(ZondaV2 main.c
        motos_zonda.h
        motos_zonda.c)
