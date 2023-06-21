cmake_minimum_required(VERSION 3.10.0)
project (HW1)
set(CMAKE_CXX_STANDARD 17)


add_library(graph src/listgraph.cpp
             src/matrixgraph.cpp
             src/arcgraph.cpp
            src/setgraph.cpp)

target_include_directories(graph PUBLIC include)

add_executable(hw1 main.cpp)
target_link_libraries(hw1 graph)