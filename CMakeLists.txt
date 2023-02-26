cmake_minimum_required(VERSION 3.15)

project(basic)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUUIRED Ture)

include_directories(include)

aux_source_directory(src/thread DIR_SRCS)
add_library(${PROJECT_NAME} ${DIR_SRCS})

add_executable(basic_test basic_test.cpp)
target_link_libraries(basic_test PUBLIC basic pthread)