/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 09:10:55 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/30 09:14:47 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstclear(t_list **lst, void (*del) (void *))
{
	t_list	*ptr;
	t_list	*temp;

	ptr = *lst;
	while (ptr != NULL)
	{
		temp = ptr->next;
		ft_lstdelone(ptr, del);
		ptr = temp;
	}
	*lst = NULL;
}
