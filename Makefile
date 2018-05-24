SRC	=	Src/Entity/AEntity.cpp		\
		Src/Entity/Character/ACharacter.cpp	\

OBJ	=	$(SRC:.c=.o)

CXXFLAGS	=	-W -Wall -Wextra -ISrc/Entity/ -ISrc/Entity/Character/

NAME	=	toto

all:	$(NAME)

$(NAME):	$(OBJ)
		g++ -o $(NAME) $(OBJ)


clean:
	rm -rf $(OBJ)
	rm -rf $(NAME)
