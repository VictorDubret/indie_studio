cmake_minimum_required(VERSION 3.7.2)
project(bomberman)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE FILE Src/*.cpp Src/*.hpp externalLibs)

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

add_executable(bomberman
        ${FILE})

if (UNIX)

add_custom_command(
        OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/libIrrlicht.a
        COMMAND
        mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/include
        COMMAND
        wget https://datapacket.dl.sourceforge.net/project/irrlicht/Irrlicht%20SDK/1.8/1.8.4/irrlicht-1.8.4.zip
        COMMAND
        unzip irrlicht-1.8.4.zip
        COMMAND
        make -C ${CMAKE_CURRENT_SOURCE_DIR}/irrlicht-1.8.4/source/Irrlicht
        COMMAND
        cp -r ${CMAKE_CURRENT_SOURCE_DIR}/irrlicht-1.8.4/include ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/
        COMMAND
        cp ${CMAKE_CURRENT_SOURCE_DIR}/irrlicht-1.8.4/lib/Linux/libIrrlicht.a ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs
        COMMAND
        rm -rf irrlicht-1.8.4
        COMMAND
        rm -f irrlicht-1.8.4.zip
        COMMENT
        "Installing Irrlicht"
        VERBATIM
)

add_custom_target(
        installIrrlicht
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/libIrrlicht.a
)

add_dependencies(bomberman installIrrlicht)


add_custom_command(
        OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/libIrrKlang.so
        COMMAND
        mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/include
        COMMAND
        wget http://www.ambiera.at/downloads/irrKlang-64bit-1.6.0.zip
        COMMAND
        unzip irrKlang-64bit-1.6.0.zip
        COMMAND
        cp -r ${CMAKE_CURRENT_SOURCE_DIR}/irrKlang-64bit-1.6.0/include ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/
        COMMAND
        cp ${CMAKE_CURRENT_SOURCE_DIR}/irrKlang-64bit-1.6.0/bin/linux-gcc-64/libIrrKlang.so ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs
        COMMAND
        rm -rf irrKlang-64bit-1.6.0
        COMMAND
        rm -f irrKlang-64bit-1.6.0.zip
        COMMENT
        "Installing IrrKlang"
        VERBATIM
)

add_custom_target(
        installIrrKlang
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/libIrrKlang.so
)
add_dependencies(bomberman installIrrKlang)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/libIrrlicht.a
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs
        )

target_compile_options(bomberman PRIVATE -W -Wall -Wextra -ggdb3 -w)

endif (UNIX)

include_directories(externalLibs/include)



link_directories(externalLibs)

target_link_libraries(bomberman
        pthread)


if(WIN32)
        TARGET_LINK_LIBRARIES(bomberman
        ${PROJECT_SOURCE_DIR}/externalLibs/Irrlicht.lib
        ${PROJECT_SOURCE_DIR}/externalLibs/irrKlang.lib
        )

endif(WIN32)

if(UNIX)
target_link_libraries(bomberman
        Irrlicht GL Xxf86vm Xext X11 Xcursor ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/libIrrKlang.so)
endif(UNIX)
