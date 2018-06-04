cmake_minimum_required(VERSION 3.9)
project(bomberman)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE FILE Src/*.cpp Src/*.hpp Src Src Src)

include_directories(Src Src/Entity Src/Entity/Character
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
        Src/Entity/Bomb/
        Src/Entity/Scenery/Explosion
	/usr/include)

add_definitions(-DDEBUG=1)

add_executable(bomberman
        ${FILE})

target_link_libraries(bomberman
        pthread Irrlicht GL Xxf86vm Xext X11 Xcursor /usr/bin/libIrrKlang.so)
