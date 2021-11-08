#include "minishell.h"

char	**fill_loop(t_token *current_token, char **cmd, int i)
{
	char	*temp;

	if (cmd[i] != NULL)
	{
		current_token->buffer.str = remove_quotes(current_token->buffer.str,
				current_token->token_type);
		temp = ft_strjoin(cmd[i], current_token->buffer.str);
		free(cmd[i]);
		cmd[i] = temp;
		cmd[i + 1] = NULL;
	}
	else
	{
		current_token->buffer.str = remove_quotes(current_token->buffer.str,
				current_token->token_type);
		cmd = array_realloc(cmd, current_token->buffer.str);
	}
	return (cmd);
}

char	**prepare_command(t_token *token, t_token **current_token,
	t_command *current_command)
{
	char	**cmd;

	initialize_command(current_command);
	(*current_token) = token;
	if ((*current_token) && (*current_token)->token_type == SPACE_SIGN)
		(*current_token) = (*current_token)->next;
	cmd = protected_malloc(2, sizeof(char *));
	(*current_token)->buffer.str = remove_quotes((*current_token)->buffer.str,
			(*current_token)->token_type);
	cmd[0] = ft_strdup((*current_token)->buffer.str);
	cmd[1] = NULL;
	(*current_token) = (*current_token)->next;
	return (cmd);
}

void	fill_command(t_token *token, t_command *current_command)
{
	int		i;
	char	**cmd;
	char	*temp;
	t_token	*current_token;

	cmd = prepare_command(token, &current_token, current_command);
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		while (current_token && ft_piperedirect(current_token->token_type) == 0
			&& current_token->token_type != SPACE_SIGN)
		{
			cmd = fill_loop(current_token, cmd, i);
			current_token = current_token->next;
		}
		if (current_token && current_token->token_type == SPACE_SIGN)
			i++;
		if (current_token && ft_piperedirect(current_token->token_type) == 0)
			current_token = current_token->next;
	}
	if (current_token)
		add_piperedirect(current_token, current_command);
	current_command->command = cmd;
}

void	malloc_cmd_next(t_command **current_cmd)
{
	(*current_cmd)->next = protected_malloc(1, sizeof(t_command));
	(*current_cmd) = (*current_cmd)->next;
	(*current_cmd)->next = NULL;
}

void	fill_commands(t_vars *vars, t_token *token, int i)
{
	t_command	*current_cmd;

	while (token)
	{
		if (token->token_type == SPACE_SIGN && token->next)
			token = token->next;
		if (token->token_type != SPACE_SIGN)
		{
			if (i++ == 0)
			{
				vars->cmd = protected_malloc(1, sizeof(t_command));
				current_cmd = vars->cmd;
				current_cmd->next = NULL;
			}
			else
				malloc_cmd_next(&current_cmd);
			fill_command(token, current_cmd);
			while (token && ft_piperedirect(token->token_type) == 0)
				token = token->next;
			while (token && ft_piperedirect(token->token_type) == 1)
				token = token->next;
		}
		else
			token = token->next;
	}
}
