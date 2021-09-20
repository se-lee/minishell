/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 13:23:47 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 09:17:25 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*strdst;
	unsigned char	*strsrc;

	i = 0;
	strdst = (unsigned char *)dst;
	strsrc = (unsigned char *)src;
	if (src == NULL && dst == NULL)
		return (dst);
	while (i < n)
	{
		strdst[i] = strsrc[i];
		i++;
	}
	return (dst);
}
