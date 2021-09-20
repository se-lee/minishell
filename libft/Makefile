# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/24 16:54:06 by acusanno          #+#    #+#              #
#    Updated: 2021/05/26 09:32:49 by acusanno         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

SRCS		= ft_isalnum.c ft_memccpy.c ft_memset.c ft_strlen.c ft_tolower.c \
			ft_isalpha.c ft_memchr.c  ft_strchr.c  ft_strncmp.c ft_toupper.c \
			ft_atoi.c ft_isascii.c ft_memcmp.c ft_strdup.c ft_strnstr.c ft_bzero.c \
			ft_isdigit.c ft_memcpy.c ft_strlcat.c ft_strrchr.c ft_calloc.c  \
			ft_isprint.c ft_memmove.c ft_strlcpy.c ft_substr.c ft_strjoin.c \
			ft_strtrim.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
			ft_putnbr_fd.c ft_strmapi.c ft_itoa.c ft_split.c ft_itoa_base.c \
			ft_min.c ft_max.c ft_itoa_pf.c get_next_line.c get_next_line_utils.c

SRCS_BONUS	= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			  ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
			  ft_lstmap.c

OBJS		= ${SRCS:.c=.o}

OBJS_BONUS	= ${SRCS_BONUS:.c=.o}

NAME		= libft.a

CC			= gcc -g3 -g

AR			= ar rc

RM			= rm -f

HEADER		= ./libft.h

CFLAGS		= -Wall -Wextra -Werror

all:		${NAME}

$(NAME):	${OBJS} ${HEADER}
			${AR} ${NAME} ${OBJS}

%.o: %.c	${HEADER}
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

bonus:		${OBJS_BONUS} ${OBJS}
			${AR} ${NAME} ${OBJS_BONUS} ${OBJS}

clean:
			${RM} ${OBJS} ${OBJS_BONUS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re bonus
