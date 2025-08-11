cmake_minimum_required(VERSION 3.10)
project(POS_system)

set(CMAKE_CXX_STANDARD 20)

add_executable(POS_system main.cpp src/sqlite3.c)

target_include_directories(POS_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})