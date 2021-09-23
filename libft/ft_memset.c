/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyounglee <seoyounglee@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 10:45:32 by acusanno          #+#    #+#             */
/*   Updated: 2021/09/21 14:20:41 by seoyounglee      ###   ########lyon.fr   */
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
