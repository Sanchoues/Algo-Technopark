cmake_minimum_required(VERSION 3.10.0)
project (HW5)
set(CMAKE_CXX_STANDARD 17)


add_library(travellingsalesman src/listgraph.cpp
                                src/minimumspanningtree.cpp
                                src/randomgraph.cpp
                                src/travellingsalesman.cpp)

target_include_directories(travellingsalesman PUBLIC include)

add_executable(hw5 main.cpp)
target_link_libraries(hw5 travellingsalesman)