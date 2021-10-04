NAME = minishell

SRCS =	minishell.c						\
		parsing/parsing.c				\
		parsing/variables.c				\
		parsing/parsing_utils.c			\
		parsing/tokenization.c			\
		parsing/tokenization_utils.c	\
		built_in/cd.c					\
		built_in/echo.c					\
		built_in/env.c					\
		built_in/exit.c					\
		built_in/export.c				\
		built_in/pwd.c					\
		built_in/unset.c				\
		built_in/execution.c			\
		built_in/envlist_util.c

GCCF = gcc -g -fsanitize=address #-Wall -Wextra -Werror

READLINE = -lreadline -L /Users/acusanno/.brew/opt/readline/lib -I/Users/acusanno/.brew/opt/readline/include

OBJS = $(addprefix bin/, ${SRCS:.c=.o})

.PHONY :	all clean fclean re libft_

bin/%.o : 	src/%.c	include/minishell.h
			mkdir -p $(dir $@)
			${GCCF} -c $< -o $@ -I include

all :		libft_ ${NAME}

$(NAME) :	${OBJS} include/minishell.h | libft/libft.a
			${GCCF} ${READLINE} libft/libft.a $(OBJS) -o $(NAME)

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
