# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 21:57:02 by ymiao             #+#    #+#              #
#    Updated: 2025/08/22 20:42:40 by ymiao            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= clang
CFLAG	= -Wall -Werror -Wextra

NAME	= miniRT

SRC_DIR	= src/
SRC		= main.c test.c \
			math_tool/vector_part1.c math_tool/vector_part2.c \
			math_tool/color.c \
			objects/objects_list.c \
			objects/sphere.c \
			objects/plane.c \
			objects/cylinder.c \
			objects/hit.c \
			event/event.c \
			render/setup.c \
			render/camera.c \
			render/ray.c \
			render/shadow.c \
			render/all_lights.c \
			render/render.c \
			utils/mem_manager.c \
			utils/ft_split.c \
			utils/ft_putstr_fd.c \

OBJ_DIR	= obj/
OBJ		= $(addprefix $(OBJ_DIR),$(SRC:.c=.o))

MLX_URL	= https://cdn.intra.42.fr/document/document/34994/minilibx-linux.tgz
MLX_TAR	= mlx.tar.gz

all : check_mlx $(NAME)

check_mlx:
	@if [ ! -d "mlx_linux" ]; then \
		echo "Mlx library not found. Downloading..."; \
		wget $(MLX_URL) -O $(MLX_TAR); \
		echo "Extracting Mlx library..."; \
		tar -xzf $(MLX_TAR); \
		mv minilibx-linux mlx_linux; \
		echo "Configuring mlx library..."; \
		cd mlx_linux && bash configure; \
		rm -f ../$(MLX_TAR); \
	fi

$(NAME): $(OBJ)
	$(CC) $(CFLAG) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -O3 -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -I/usr/include -Imlx_linux -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re check_mlx
