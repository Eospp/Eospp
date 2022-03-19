project(kernel_test)

cmake_minimum_required(VERSION 3.4)

include_directories(${CMAKE_PROJECT_DIR}../include)
include_directories(${CMAKE_PROJECT_DIR}../../estl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")

find_package(GTest REQUIRED)

if(GTest_FOUND)
message(STATUS "found gtest,subsequently build kernel unit test")
enable_testing()
else()
message(FATAL_ERROR  "can't find gtest,please install it first")
endif()

aux_source_directory(../src/util util_srcs)

add_library(kernel SHARED
            ${util_srcs})
            
set(TEST_LISTS
    string_test
    literals_test)

foreach(Test ${TEST_LISTS})
    message(STATUS "build ${Test}")
    add_executable(${Test} ${Test}.cpp)
    target_link_libraries(${Test} kernel GTest::GTest GTest::Main)    
    add_test(NAME ${Test} COMMAND ${Test})
endforeach()


