/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 12:54:20 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 10:15:54 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

size_t	len(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcat(char *dest, char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[j] && j < dstsize)
		j++;
	while (src[i] && i + j + 1 < dstsize)
	{
		dest[i + j] = src[i];
		i++;
	}
	if (j != dstsize)
		dest[j + i] = '\0';
	return (j + len(src));
}
