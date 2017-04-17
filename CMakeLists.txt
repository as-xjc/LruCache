cmake_minimum_required(VERSION 2.8)
project(Lru)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES sample.cpp lru.hpp)
add_executable(Lru ${SOURCE_FILES})
