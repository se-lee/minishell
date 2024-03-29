/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:02 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:42:04 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace(char *full, char *placeholder, char *real)
{
	int		i;
	int		j;
	int		len;
	char	*final;

	len = ft_strlen(full) + ft_strlen(real) - ft_strlen(placeholder);
	final = protected_malloc((len + 1), sizeof(char));
	i = 0;
	while (ft_strncmp(&full[i], placeholder, ft_strlen(placeholder)) != 0)
	{
		final[i] = full[i];
		i++;
	}
	j = 0;
	while (j < (int)ft_strlen(real))
	{
		final[i + j] = real[j];
		j++;
	}
	j = i + j;
	i += ft_strlen(placeholder);
	while (j < len)
		final[j++] = full[i++];
	final[j] = '\0';
	return (final);
}

int	find_space(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str == NULL)
		return (0);
	while (str[i])
	{
		if (str[i] == ' ')
		{
			while (str[i] == ' ')
				i++;
			if (str[i] && str[i] != ' ')
				j++;
		}
		i++;
	}
	return (j);
}

void	printf_commands(t_vars *vars)
{
	int			i;
	t_command	*current_cmd;

	current_cmd = vars->cmd;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->command[i] != NULL)
		{
			printf("command[%d] = %s\n", i, current_cmd->command[i]);
			i++;
		}
		current_cmd = current_cmd->next;
	}
}

t_token	*remove_token(t_vars *vars, t_token *token)
{
	t_token	*current_token;

	if (vars->first == token)
	{
		vars->first = token->next;
		free(token->buffer.str);
		free(token);
		return (vars->first);
	}
	else
	{
		current_token = vars->first;
		while (current_token->next && current_token->next != token)
			current_token = current_token->next;
		current_token->next = token->next;
		free(token->buffer.str);
		free(token);
		return (current_token->next);
	}
}

void	parsing(t_vars *vars, char *str)
{
	t_token	*current_token;

	tokenization(vars, str);
	current_token = vars->first;
	while (current_token && vars->error != -1)
	{
		if (check_error(current_token) == 0
			&& check_error2(vars, current_token) == 0)
		{
			if (current_token->token_type == WORD
				|| current_token->token_type == QUOTE)
			{
				current_token = replace_env(vars, current_token);
			}
			else
				current_token = current_token->next;
		}
		else
			display_syntax_error(vars, current_token);
	}
	fill_redirect(vars, 0);
	fill_commands(vars, vars->first, 0);
}
