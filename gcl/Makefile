
NAME	:= test.a

SRC		:= get_next_line.c get_next_line_utils.c
OBJS	:= $(SRC:%.c=%.o)

SRCBONUS	:= get_next_line_bonus.c get_next_line_utils_bonus.c
OBJBONUS	:= $(SRCBONUS:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJS)
	ar rc $@ $(OBJS)

bonus : .bonus

.bonus : $(OBJBONUS) $(OBJS)
	ar rc $(NAME) $(OBJS) $(OBJBONUS)
	@touch .bonus

%.o : %.c
	cc -Wall -Werror -Wextra -c $< -o $@

clean :
	rm -rf $(OBJS) $(OBJBONUS) .bonus

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re bonus