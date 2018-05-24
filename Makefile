SRC	=	Src/Entity/AEntity.cpp		\
		test.cpp			\
		Src/Entity/Character/ACharacter.cpp	\

OBJ	=	$(SRC:.cpp=.o)

CXXFLAGS	=	-W -Wall -Wextra -ISrc/Entity/ -ISrc/Entity/Character/

NAME	=	toto

all:	$(NAME)

$(NAME):	$(OBJ)
		g++ -o $(NAME) $(OBJ) -lpthread


clean:
	rm -rf $(OBJ)
	rm -rf $(NAME)
