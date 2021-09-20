/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 13:08:14 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 09:18:56 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

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
