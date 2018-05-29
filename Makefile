SRC	=	Src/Entity/AEntity.cpp		\
		test.cpp			\
		Src/Entity/Character/ACharacter.cpp	\
		Src/Tools/Thread.cpp	\
		Src/Tools/ThreadPool.cpp	\
		Src/Entity/PowerUp/APowerUp.cpp	\
		Src/Entity/PowerUp/BombUp/BombUp.cpp	\
		Src/Entity/PowerUp/FireUp/FireUp.cpp	\
		Src/Entity/PowerUp/SpeedUp/SpeedUp.cpp	\
		Src/Entity/PowerUp/WallPass/WallPass.cpp

OBJ	=	$(SRC:.cpp=.o)

CC	=	g++
CXX	=	g++

CXXFLAGS	=	-W -Wall -Wextra -ISrc/Entity/ -ISrc/Entity/Character/ -ISrc/Tools \
				-ISrc/Entity/PowerUp -ISrc/Entity/PowerUp/BombUp -ISrc/Entity/PowerUp/FireUp \
				-ISrc/Entity/PowerUp/SpeedUp -ISrc/Entity/PowerUp/WallPass

LDFLAGS	=	-lpthread

NAME	=	toto

all:	$(NAME)

$(NAME):	$(OBJ)
		$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)


clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

exec: re
	./$(NAME)

debug: CXXFLAGS += -ggdb3 -DDEBUG=1

debug: re

dexec: debug exec