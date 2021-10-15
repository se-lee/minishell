NAME = minishell

SRCS =	minishell.c						\
		parsing/parsing.c				\
		parsing/variables.c				\
		parsing/parsing_utils.c			\
		parsing/tokenization.c			\
		parsing/tokenization_utils.c	\
		parsing/misc_utils.c			\
		parsing/envlist_utils.c			\
		built_in/cd.c					\
		built_in/echo.c					\
		built_in/env.c					\
		built_in/exit.c					\
		built_in/export.c				\
		built_in/export_utils.c			\
		built_in/pwd.c					\
		built_in/unset.c				\
		cmd_exec/execution.c			\
		cmd_exec/test_functions.c		\
<<<<<<< HEAD
		cmd_exec/cmd_utils.c

GCCF = gcc -g -fsanitize=address #-Wall -Wextra -Werror
=======
		cmd_exec/cmd_utils.c			\
		built_in/export_utils.c			\

GCCF = gcc -g3 -fsanitize=address# -Wall -Wextra -Werror
>>>>>>> 721131659ce783257624439df0a9cefd63eb54de

READLINE = -lreadline -L/usr/local/lib -I/usr/local/include

#-L /Users/acusanno/.brew/opt/readline/lib -I/Users/acusanno/.brew/opt/readline/include

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
