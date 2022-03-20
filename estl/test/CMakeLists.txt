project(estl_test)

cmake_minimum_required(VERSION 3.4)

find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/../)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")

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
    vector_test
    defer_test
    list_test
    deque_test
    queue_test
    stack_test
    string_view_test
    rb_tree_test
    set_test
    algorithm_test
    array_test
    bitset_test
    )

set(MEMORY_TESTS
    memory_test
    vector_test
    list_test
    deque_test
    queue_test
    stack_test
    rb_tree_test
    set_test
    )

    foreach(Test ${TEST_LISTS})
    message(STATUS "build ${Test}")
    add_executable(${Test} ${Test}.cpp)
    target_link_libraries(${Test} GTest::GTest GTest::Main)    
    add_test(NAME ${Test} COMMAND ${Test})
endforeach()

find_program(CMAKE_MEMORYCHECK_COMMAND valgrind)
set(memcheck_command ${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full --error-exitcode=1)

foreach(src ${MEMORY_TESTS})
	add_test(${src}-memory-check ${memcheck_command} ./${src})
endforeach()