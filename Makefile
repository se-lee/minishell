NAME = minishell

SRCS =	minishell.c						\
		free_utils.c					\
		signals.c						\
		parsing/parsing.c				\
		parsing/variables.c				\
		parsing/parsing_errors.c				\
		parsing/variable_utils.c		\
		parsing/parsing_utils.c			\
		parsing/tokenization.c			\
		parsing/tokenization_utils.c	\
		parsing/misc_utils.c			\
		parsing/envlist_utils.c			\
		parsing/envlist_utils2.c		\
		parsing/redirect.c				\
		parsing/commands.c				\
		parsing/commands_utils.c		\
		built_in/cd.c					\
		built_in/cd_utils.c				\
		built_in/echo.c					\
		built_in/env.c					\
		built_in/exit_utils.c			\
		built_in/exit.c					\
		built_in/export.c				\
		built_in/export_utils.c			\
		built_in/pwd.c					\
		built_in/unset.c				\
		cmd_exec/cmd_utils.c			\
		cmd_exec/envlist_utils.c		\
		cmd_exec/execution_utils.c		\
		cmd_exec/execution.c			\
		cmd_exec/heredoc.c				\
		cmd_exec/pipe.c					\
		cmd_exec/redirection.c			\

GCCF = gcc -g  -fsanitize=address #-Wall -Wextra -Werror

READLINE = -lreadline -L/usr/local/lib -I/usr/local/include

OBJS = $(addprefix bin/, ${SRCS:.c=.o})

.PHONY :	all clean fclean re libft_

bin/%.o : 	src/%.c	include/minishell.h
			@mkdir -p $(dir $@)
			${GCCF} -c $< -o $@ -Iinclude -Ilibft

all :		libft_ ${NAME}

$(NAME) :	${OBJS} include/minishell.h | libft/libft.a
			${GCCF} ${READLINE} $(OBJS) libft/libft.a -o $(NAME)

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
