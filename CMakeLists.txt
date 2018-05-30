cmake_minimum_required(VERSION 3.9)
project(cpp_indie_studio)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE FILE Src/*.cpp Src/*.hpp Src)

include_directories(
        Src
        Src/Entity
        Src/Entity/Character
        Src/Tools
        Src/GameEngine
        Src/Entity/PowerUp/BombUp
        Src/Entity/PowerUp/FireUp
        Src/Entity/PowerUp/WallPass
        Src/Entity/PowerUp/SpeedUp
        Src/Entity/PowerUp
        Src/Entity/Scenery/Wall
        Src/Entity/Scenery/UnbreakableWall
        Src/Entity/Scenery/Explosion)

add_definitions(-DDEBUG=1)

add_executable(cpp_indie_studio
        ${FILE} Src/Entity/Scenery/Explosion/Explosion.cpp Src/Entity/Scenery/Explosion/Explosion.hppa)

target_link_libraries(cpp_indie_studio
        pthread)
