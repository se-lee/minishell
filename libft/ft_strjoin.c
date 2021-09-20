/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 08:51:43 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 10:37:10 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	int		i;
	char	*s3;

	i = -1;
	if (s1 == 0 || s2 == 0)
		return (0);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (0);
	while (++i < len1)
	{
		s3[i] = (char)s1[i];
	}
	i = -1;
	while (++i < len2)
	{
		s3[len1 + i] = (char)s2[i];
	}
	s3[len1 + i] = '\0';
	return (s3);
}
