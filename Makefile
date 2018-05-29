SRC	=	Src/Entity/AEntity.cpp		\
		Src/main.cpp			\
		Src/Entity/Character/ACharacter.cpp	\
		Src/Tools/Thread.cpp		\
		Src/Tools/ThreadPool.cpp	\
		Src/Entity/Bomb/Timer.cpp	\
		Src/Entity/Bomb/Bomb.cpp

OBJ	=	$(SRC:.cpp=.o)

CXX	=	g++
CC	=	clang++

CXXFLAGS	=	-W -Wall -Wextra -ISrc/Entity/ -ISrc/Entity/Character/ -ISrc/Tools

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
