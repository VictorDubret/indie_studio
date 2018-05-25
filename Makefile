SRC	=	Src/Entity/AEntity.cpp		\
		test.cpp			\
		Src/Entity/Character/ACharacter.cpp	\
		Src/Tools/Thread.cpp	\
		Src/Tools/ThreadPool.cpp

OBJ	=	$(SRC:.cpp=.o)

CC	=	g++
CXX	=	clang++

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