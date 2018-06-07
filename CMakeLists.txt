cmake_minimum_required(VERSION 3.7.2)
project(bomberman)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE FILE Src/*.cpp Src/*.hpp Src Src Src)

include_directories(Src Src/Entity Src/Entity/Character
	    Src
		Src/MapGenerator
        Src/Entity
        Src/Entity/Character
		Src/Entity/Character/AI
        Src/Tools
        Src/GameEngine
		Src/MapGenerator
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

target_compile_options(bomberman PRIVATE -W -Wall -Wextra -ggdb3)
target_link_libraries(bomberman
        pthread Irrlicht GL Xxf86vm Xext X11 Xcursor /usr/bin/libIrrKlang.so)
