/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 08:51:43 by acusanno          #+#    #+#             */
/*   Updated: 2021/09/21 11:20:04 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin_char(char const *s1, char c)
{
	int		len1;
	int		i;
	char	*s3;

	i = -1;
	if (s1 == 0)
		return (0);
	len1 = ft_strlen(s1);
	s3 = malloc(sizeof(char) * (len1 + 2));
	if (!s3)
		return (0);
	while (++i < len1)
	{
		s3[i] = (char)s1[i];
	}
	s3[i + 1] = c;
	s3[i + 2] = '\0';
	return (s3);
}
