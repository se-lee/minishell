NAME = minishell

SRCS =	minishell.c

GCCF = gcc -Wall -Werror -Wextra

OBJS = ${SRCS:.c=.o}

_END = \033[0m
_DIM = \033[2m
_YELLOW = \033[33m
_GREEN = \033[92m
_RED = \033[91m
_ROSE = \033[95m
_CYAN = \033[96m
.PHONY :	all clean fclean re libft_

%.o : 		%.c		minishell.h
			${GCCF} -c $< -o $@ 

all :		libft_ ${NAME}

$(NAME) :	${OBJS} minishell.h | libft/libft.a
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


