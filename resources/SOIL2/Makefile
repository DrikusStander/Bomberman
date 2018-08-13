# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/05/09 16:44:45 by pmatle            #+#    #+#              #
#    Updated: 2018/08/10 11:36:28 by cnolte           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libSOIL2.a

CC = gcc

SRC = etc1_utils.c image_DXT.c image_helper.c SOIL2.c

OBJ = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME):
	@$(CC) -c $(SRC)
	@ar rcs $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

time:
	make clean && make -j2 && make fclean && make -j2 && make re && ls -l libft.a
.PHONY: clean fclean all re
