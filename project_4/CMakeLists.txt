cmake_minimum_required(VERSION 3.30)
project(project_4)

set(CMAKE_CXX_STANDARD 20)

add_executable(project_4 main.cpp
        genetic_algorithm.cpp
        genetic_algorithm.h
        tsp_utils.cpp
        tsp_utils.h
        config.h)
