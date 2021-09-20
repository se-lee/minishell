/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 12:58:20 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/31 08:55:59 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

int		get_next_line(int fd, char **line);
int		ft_strlen_gnl(const char *str);
int		ft_isline_break(const char *s);
char	*ft_strjoin2(char const *s1, char const *s2);
char	*ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize);
char	*ftgsubstr(char const *s, int start, size_t len, int *ret2);
char	*after_lb(char *str, int *ret);
char	*before_lb(char *str, int *ret);
#endif
