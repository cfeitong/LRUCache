cmake_minimum_required(VERSION 3.2)

project(LRUCache)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-Wall -ansi -Wno-deprecated -pthread -g)

add_executable(LRU_TEST test_lru.cpp)
add_executable(RWLOCK_TEST test_rwlock.cpp)

# setup googletest
add_subdirectory(gtest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})

add_dependencies(LRU_TEST gtest)
add_dependencies(RWLOCK_TEST gtest)

target_link_libraries(LRU_TEST gtest gtest_main)
add_test(lrutest LRU_TEST)

target_link_libraries(RWLOCK_TEST gtest gtest_main)
add_test(rwlocktest RWLOCK_TEST)

