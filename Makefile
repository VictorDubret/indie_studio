SRC		=		Src/Main.cpp					\
				Src/Entity/AEntity.cpp				\
				Src/Entity/Character/ACharacter.cpp		\
				Src/Tools/Thread.cpp				\
				Src/Tools/ThreadPool.cpp			\
				Src/Entity/PowerUp/APowerUp.cpp			\
				Src/Entity/PowerUp/BombUp/BombUp.cpp		\
				Src/Entity/PowerUp/FireUp/FireUp.cpp		\
				Src/Entity/PowerUp/SpeedUp/SpeedUp.cpp		\
				Src/Entity/PowerUp/WallPass/WallPass.cpp

SRC_IRRLICHT	=		Src/GameEngine/ManageIrrlicht.cpp		\
				Src/GameEngine/EventManager.cpp			\
				Src/GameEngine/ManageObject.cpp			\

OBJ		=		$(SRC:.cpp=.o)					\
				$(SRC_IRRLICHT:.cpp=.o)

CC		=		g++

CXX		=		clang++

CXXFLAGS	=		-W -Wall -Wextra				\
				-I Src/Entity/					\
				-I Src/Entity/Character/			\
				-I Src/Tools/					\
				-I Src/Entity/PowerUp/				\
				-I Src/Entity/PowerUp/BombUp/			\
				-I Src/Entity/PowerUp/FireUp/			\
				-I Src/Entity/PowerUp/SpeedUp/			\
				-I Src/Entity/PowerUp/WallPass/			\
				-I Src/GameEngine/				\
				-std=c++11

LDFLAGS		=		-lpthread -lIrrlicht -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor

NAME		=		toto

all:				$(NAME)

$(NAME):			$(OBJ)
				$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
				rm -rf $(OBJ)

fclean: 			clean
				rm -rf $(NAME)

re: 				fclean all

exec: 				re
				./$(NAME)

debug: 				CXXFLAGS += -ggdb3 -DDEBUG=1

debug: 				re

dexec: 				debug exec

#multilib handling                                                                                    
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features                                                                           
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif

