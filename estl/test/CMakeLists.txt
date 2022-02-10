project(estl_test)

cmake_minimum_required(VERSION 3.4)

find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/../)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(GTest_FOUND)
message(STATUS "found gtest,subsequently build estl unit test")
enable_testing()
else()
message(FATAL_ERROR  "can't find gtest,please install it first")
endif()

set(TEST_LISTS 
    type_traits_test
    utility_test
    atomic_test
    pair_test
    any_test
    tuple_test
    functional_test
    memory_test
    )

foreach(Test ${TEST_LISTS})
    message(STATUS "build ${Test}")
    add_executable(${Test} ${Test}.cpp)
    target_link_libraries(${Test} GTest::GTest GTest::Main)    
    add_test(NAME ${Test} COMMAND ${Test})
endforeach()
