/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:41:50 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:41:52 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char c)
{
	if (ft_isalnum(c) == 1 || c == '_')
		return (1);
	return (0);
}

char	*find_variable(char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (str[i] && (is_valid(str[i]) == 1 || (i == 0 && str[i] == '$')
			|| (i == 1 && str[i] == '?')))
		i++;
	var = protected_malloc((i + 1), sizeof(char));
	i = 0;
	while (str[i] && (is_valid(str[i]) || (i == 0 && str[i] == '$')
			|| (i == 1 && str[i] == '?')))
	{
		var[i] = str[i];
		if (i == 1 && ft_isdigit(str[i]) == 1)
		{
			i++;
			break ;
		}
		i++;
	}
	var[i] = '\0';
	return (var);
}

void	malloc_token_next(t_token **current_token)
{
	(*current_token)->next = protected_malloc(1, sizeof(t_token));
	(*current_token) = (*current_token)->next;
	(*current_token)->next = NULL;
}
