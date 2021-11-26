/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:06 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:42:07 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspecial(char c)
{
	if ((c == '"') || (c == '\'') || (c == ' ')
		|| (c == '<') || (c == '>') || (c == '|'))
		return (1);
	else
		return (0);
}

int	ft_isupper(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

void	ft_comandadd_back(t_token **alst, t_token *new)
{
	t_token	*ptr;

	ptr = *alst;
	if (!(*alst))
		*alst = new;
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
		new->next = NULL;
	}
}

int	ft_piperedirect(int token_type)
{
	if (token_type == PIPE_SIGN || token_type == REDIRECT_RIGHT
		|| token_type == REDIRECT_LEFT)
		return (1);
	return (0);
}
