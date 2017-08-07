cmake_minimum_required(VERSION 3.2)

project(LRUCache)

find_package(Threads REQUIRED)

set(PROJECT_TEST ${CMAKE_PROJECT_NAME}Test)
set(LRUCacheTestSrc test.cpp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-Wall -ansi -Wno-deprecated -pthread)

add_executable(${PROJECT_TEST} ${LRUCacheTestSrc})

# setup googletest
add_subdirectory(gtest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})
add_dependencies(${PROJECT_TEST} gtest)
target_link_libraries(${PROJECT_TEST} gtest gtest_main)
add_test(lrutest ${PROJECT_TEST})


