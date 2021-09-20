/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 10:45:32 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 09:19:01 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	char			*str;

	i = 0;
	str = (char *)b;
	while (i != len)
	{
		str[i] = c;
		i++;
	}
	b = (void *)str;
	return (b);
}
