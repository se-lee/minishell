NAME = minishell

SRCS =	minishell.c		\
		parsing.c		\
		parsing_utils.c	\
		tokenization.c	\

GCCF = gcc -g -fsanitize=address # -Wall -Wextra -Werror

OBJS = $(addprefix bin/, ${SRCS:.c=.o})

_END = \033[0m
_DIM = \033[2m
_YELLOW = \033[33m
_GREEN = \033[92m
_RED = \033[91m
_ROSE = \033[95m
_CYAN = \033[96m
.PHONY :	all clean fclean re libft_

bin/%.o : 		src/%.c		include/minishell.h
			mkdir -p $(dir $@)
			${GCCF} -c $< -o $@ -I include

all :		libft_ ${NAME}

$(NAME) :	${OBJS} include/minishell.h | libft/libft.a
			${GCCF} libft/libft.a $(OBJS) -o $(NAME)

libft_ :
			$(MAKE) -C libft

libft/libft.a :
			$(MAKE) -C libft

clean :
			rm -f ${OBJS}
			${MAKE} -C libft/ clean

fclean :	clean
			rm -f ${NAME}
			${MAKE} -C libft/ fclean

re :		fclean all


