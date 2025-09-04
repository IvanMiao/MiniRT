# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 21:57:02 by ymiao             #+#    #+#              #
#    Updated: 2025/09/05 01:05:30 by ymiao            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= clang
CFLAG	= -Wall -Werror -Wextra -O3

NAME	= miniRT_bonus

SRC_DIR	= src/
SRC		= main_bonus.c \
			math_tool/vector_part1_bonus.c math_tool/vector_part2_bonus.c \
			math_tool/color_bonus.c \
			objects/objects_list_bonus.c \
			objects/sphere_bonus.c \
			objects/plane_bonus.c \
			objects/cylinder_bonus.c \
			objects/hit_bonus.c \
			objects/cone_bonus.c \
			parsing/assignment_amb_bonus.c \
			parsing/assignment_obj_bonus.c \
			parsing/assignment_cam_bonus.c \
			parsing/utils_atof_bonus.c \
			parsing/utils_parse_bonus.c \
			parsing/parser_bonus.c \
			event/event_bonus.c \
			render/camera_bonus.c \
			render/ray_bonus.c \
			render/shadow_bonus.c \
			render/light_list_bonus.c \
			render/all_lights_bonus.c \
			render/render_bonus.c \
			utils/mem_manager_bonus.c \
			utils/ft_split_bonus.c \
			utils/ft_split_space_bonus.c \
			utils/ft_putstr_fd_bonus.c \
			utils/ft_strcmp_bonus.c \
			utils/ft_atoi_bonus.c \
			utils/ft_isspace_bonus.c \
			utils/ft_isdigit_bonus.c \
			utils/ft_error_bonus.c \
			utils/get_next_line_bonus.c \
			utils/get_next_line_utils_bonus.c

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
	$(CC) $(CFLAG) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAG) -I/usr/include -Imlx_linux -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re check_mlx
