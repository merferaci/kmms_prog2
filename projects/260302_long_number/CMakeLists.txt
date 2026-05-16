cmake_minimum_required(VERSION 3.14)
project(260302_long_number VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

add_library(long_number STATIC
    ${SRC_DIR}/long_number.cpp
    ${SRC_DIR}/long_number.hpp
)

target_include_directories(long_number PUBLIC ${SRC_DIR})

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_long_number
    ${TEST_DIR}/test_long_number.cpp
)

target_link_libraries(test_long_number 
    long_number 
    gtest_main
)

target_include_directories(test_long_number PRIVATE ${SRC_DIR})

add_test(NAME LongNumberTest COMMAND test_long_number)