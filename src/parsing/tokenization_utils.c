/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:41:56 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:41:58 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_char(t_token *token, char c)
{
	char	*str;

	token->buffer.len++;
	str = ft_strjoin_char(token->buffer.str, c);
	free(token->buffer.str);
	token->buffer.str = str;
}

void	update_token(t_token *token, char *var, char *value)
{
	char	*string;

	string = replace(token->buffer.str, var, value);
	free(var);
	free(token->buffer.str);
	token->buffer.str = string;
	token->buffer.len = ft_strlen(token->buffer.str);
}
