cmake_minimum_required(VERSION 3.24)
project(project_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(project_3 main.cpp tsp_utils.cpp tsp_utils.h tabu_search.cpp tabu_search.h config.h)
