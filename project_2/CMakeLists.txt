cmake_minimum_required(VERSION 3.24)

project(project_2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

add_executable(project_2 main.cpp tsp_utils.cpp tsp_utils.h branch_and_bound.cpp branch_and_bound.h)

file(COPY ${CMAKE_SOURCE_DIR}/tsp/ DESTINATION ${CMAKE_BINARY_DIR}/tsp/)

configure_file(${CMAKE_SOURCE_DIR}/config.txt ${CMAKE_BINARY_DIR}/config.txt COPYONLY)

