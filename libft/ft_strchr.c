/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 16:48:06 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 10:14:50 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	ch;
	size_t	len;

	len = ft_strlen((char *)s);
	ch = (char)c;
	i = 0;
	while (i <= len)
	{
		if (s[i] == ch)
			return ((char *)&s[i]);
		i++;
	}
	return (0);
}
