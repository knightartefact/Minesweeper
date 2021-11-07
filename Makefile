##
## EPITECH PROJECT, 2021
## makefile
## File description:
## makefile
##
CCFLGSLIN	=	gcc -g

CCFLGSWIN	=	x86_64-w64-mingw32-gcc --static

SRC		=	minesweeper.c

ERRFLGS		=	-Werror -Wall -Wextra

CSFML_LIB	=	-lcsfml-system -lcsfml-window -lcsfml-graphics -lm

NAME	=	minesweeper

all: $(NAME)

$(NAME):
	$(CCFLGSWIN) -o $(NAME) $(SRC) $(CSFML_LIB) -I.

clean:
	rm -f vgcore.* *~ '#'*

fclean:	clean
	rm -f $(NAME)

re: fclean all
