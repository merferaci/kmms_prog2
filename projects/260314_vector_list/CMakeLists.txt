cmake_minimum_required(VERSION 3.14)
project(containers)

set(CONTAINERS_LIB "containers_lib")
add_library(${CONTAINERS_LIB} STATIC)

add_subdirectory(src)
add_subdirectory(test)