cmake_minimum_required(VERSION 3.9)
project(cpp_indie_studio)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE FILE Src/*.cpp Src/*.hpp test.cpp Src)

include_directories(Src Src/Entity Src/Entity/Character
        Src/Tools
        Src/Entity/PowerUp/BombUp
        Src/Entity/PowerUp/FireUp
        Src/Entity/PoserUp/WallPass
        Src/Entity/PoserUp/SpeedUp
        Src/Entity/PowerUp)

add_executable(cpp_indie_studio
        ${FILE})

target_link_libraries(cpp_indie_studio
        pthread)
