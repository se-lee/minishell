/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 09:03:22 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/01 09:35:21 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*after_lb(char *str, int *ret)
{
	int		i;
	char	*str_return;
	int		ret2;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!(str[i]))
	{
		free(str);
		return (NULL);
	}
	str_return = ftgsubstr(str, i + 1, (ft_strlen(str) - i), &ret2);
	if (ret2 == -1)
	{
		*ret = -1;
		return (NULL);
	}
	free(str);
	return (str_return);
}

char	*before_lb(char *str, int *ret)
{
	int		i;
	char	*str_return;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	str_return = malloc(sizeof(char) * (i + 1));
	if (!str_return)
	{
		*ret = -1;
		return (NULL);
	}
	return (ft_strlcpy_gnl(str_return, str, i + 1));
}

int	buff_change(char **buffer, char ***line, char **tmp)
{
	int		ret;

	ret = 0;
	free(*tmp);
	if (*buffer != NULL)
	{
		**line = before_lb(*buffer, &ret);
		*buffer = after_lb(*buffer, &ret);
		if (ret == -1)
			return (-1);
	}
	return (0);
}

int	gnl_while(char **buffer, char **tmp, int *read_return, int fd)
{
	while (!ft_isline_break(*buffer) && *read_return != 0)
	{
		*read_return = read(fd, *tmp, BUFFER_SIZE);
		if (*read_return == -1)
		{
			free(*tmp);
			return (-1);
		}
		(*tmp)[*read_return] = '\0';
		*buffer = ft_strjoin2(*buffer, *tmp);
		if (*buffer == 0)
			return (-1);
	}
	return (0);
}

int	get_next_line(int fd, char **line)
{
	char			*tmp;
	static char		*buffer;
	int				read_return;

	tmp = malloc(BUFFER_SIZE + 1);
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || (!tmp))
		return (-1);
	read_return = 1;
	if (gnl_while(&buffer, &tmp, &read_return, fd) == -1)
		return (-1);
	if (buff_change(&buffer, &line, &tmp) == -1)
		return (-1);
	if (read_return == 0)
		free(buffer);
	if (read_return == 0)
		return (0);
	return (1);
}
