cmake_minimum_required(VERSION 3.24)
project(project_1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

add_executable(project_1
        main.cpp
        config.cpp config.h
        tsp_utils.cpp tsp_utils.h
        bruteforce.cpp bruteforce.h
        nearestneighbor.cpp nearestneighbor.h
        randomalgorithm.cpp randomalgorithm.h
        )

file(COPY ${CMAKE_SOURCE_DIR}/tsp/ DESTINATION ${CMAKE_BINARY_DIR}/tsp/)

configure_file(${CMAKE_SOURCE_DIR}/config.txt ${CMAKE_BINARY_DIR}/config.txt COPYONLY)
