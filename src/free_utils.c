/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:41:37 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:41:40 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	free(token->buffer.str);
	token->buffer.len = 0;
	token->token_type = 0;
	token->next = NULL;
	free(token);
}

void	free_tokens(t_vars *vars)
{
	t_token	*current_token;
	t_token	*next;

	current_token = vars->first;
	while (current_token)
	{
		next = current_token->next;
		free_token(current_token);
		current_token = next;
	}
}

void	free_commands(t_vars *vars)
{
	t_command	*current_cmd;
	t_command	*next;

	current_cmd = vars->cmd;
	if (current_cmd)
	{
		while (current_cmd->next)
		{
			next = current_cmd->next;
			free_array(current_cmd->command);
			free(current_cmd);
			current_cmd = next;
		}
	}
	if (current_cmd)
	{
		free_array(current_cmd->command);
		free(current_cmd);
	}
}

void	free_inout(t_vars *vars)
{
	t_redirect	*current_inout;
	t_redirect	*temp_next;

	current_inout = vars->inout;
	while (current_inout)
	{
		temp_next = current_inout->next;
		free(current_inout->filename);
		free(current_inout);
		current_inout = temp_next;
	}
	vars->inout = NULL;
}

void	loop_free(t_vars *vars)
{
	free_tokens(vars);
	free_commands(vars);
	free_inout(vars);
}
