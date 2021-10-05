/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 13:08:14 by acusanno          #+#    #+#             */
/*   Updated: 2021/10/04 13:01:10 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	i = 0;
	if (!src && !dst)
		return (0);
	if (src < dst)
	{
		while (len > 0)
		{
			((char *)dst)[len - 1] = ((char *)src)[len - 1];
			len--;
		}
	}
	else
	{
		while (i < len)
		{
			((char *)dst)[i] = ((char *)src)[i];
			i++;
		}
	}
	return (dst);
}

/*
void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	char	*strdst;
	char	*strsrc;

	i = 0;
	strdst = (char *)dst;
	strsrc = (char *)src;
	if (src == NULL && dst == NULL)
		return (dst);
	if (src < dst)
	{
		while (len - i != 0)
			(strdst[len - i - 1] = strsrc[len - i - 1] && i++);
	}
	else
	{
		while (i < len)
			(strdst[i] = strsrc[i] && i++);
	}
	return (dst);
}
*/