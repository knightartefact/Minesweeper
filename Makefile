##
## EPITECH PROJECT, 2021
## makefile
## File description:
## makefile
##

SRC		=	minesweeper.c

ERRFLGS		=	-Werror -Wall -Wextra

CSFML_LIB	=	-lcsfml-system -lcsfml-window -lcsfml-graphics -lm

NAME	=	minesweeper

all: $(NAME)

$(NAME):
	gcc -g -o $(NAME) $(SRC) $(CSFML_LIB)

clean:
	rm -f vgcore.* *~ '#'*

fclean:	clean
	rm -f $(NAME)

re: fclean all
