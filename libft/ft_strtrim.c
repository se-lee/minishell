/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 14:29:54 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 10:17:40 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_isinset(const char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*start;
	char	*end;
	char	*s3;
	int		len;

	if (s1 == 0)
		return (0);
	len = ft_strlen(s1);
	start = (char *)s1;
	end = start + ft_strlen(s1) - 1;
	while (*start && ft_isinset(*start, set) == 1)
		start++;
	while (len && ft_isinset(*end, set) == 1)
	{
		end--;
		len--;
	}
	end++;
	if (end - start < 0)
		return (ft_strdup(""));
	s3 = malloc(sizeof(char) * (end - start + 1));
	if (!s3)
		return (0);
	ft_strlcpy(s3, start, end - start + 1);
	return (s3);
}
