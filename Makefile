# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 21:57:02 by ymiao             #+#    #+#              #
#    Updated: 2025/08/12 16:54:46 by ymiao            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
CFLAG	= -Wall -Werror -Wextra

NAME	= miniRT

SRC_DIR	= src/
SRC		= main.c \
			utils/ft_putchar.c

OBJ_DIR	= obj/
OBJ		= $(addprefix $(OBJ_DIR),$(SRC:.c=.o))

all : $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -I/usr/include -Imlx_linux -O3 -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
