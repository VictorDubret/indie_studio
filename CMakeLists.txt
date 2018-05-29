cmake_minimum_required(VERSION 2.8)

project(cpp_indie_studio)

link_libraries(pthread)

file (GLOB_RECURSE SOURCES
        Src/main.cpp
        Src/)

add_executable(bomberman ${SOURCES})

include_directories(Src/Entity/Character
        Src/Entity/
        Src/Tools
        Src/)