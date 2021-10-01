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

int	check_error(t_token *token)
{
	if (token->buffer.len > 1 && token->token_type == PIPE_SIGN)
		return (-1);
	if ((token->token_type == QUOTE || token->token_type == SINGLE_QUOTE)
		&& token->quote_nb != 2)
		return (-1);
	if (token->buffer.len > 2 && ft_piperedirect(token->token_type) == 1)
		return (-1);
	if (ft_piperedirect(token->token_type) == 1
		&& ft_piperedirect(token->next->token_type) == 1)
		return (-1);
	return (0);
}

char	*remove_quotes(char *original, int token_type)
{
	int		i;
	char	*new;

	i = 0;
	if (token_type == QUOTE || token_type == SINGLE_QUOTE)
	{
		new = protected_malloc(ft_strlen(original), sizeof(char));
		while (i < (ft_strlen(original) - 2))
		{
			new[i] = original[i + 1];
			i++;
		}
		new[i] = '\0';
		free(original);
		return (new);
	}
	else
		return (original);
}

void	fill_command(t_token *token, t_command *current_command)
{
	int		i;
	char 	**cmd;
	t_token *current_token;

	current_command->pipe = 0;
	current_command->redirect_left = 0;
	current_command->redirect_right = 0;
	i = 0;
	current_token = token;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		i++;
		current_token = current_token->next;
	}
	cmd = protected_malloc(i + 1, sizeof(char *));
	i = 0;
	current_token = token;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		cmd[i] = ft_strdup(current_token->buffer.str);
		cmd[i] = remove_quotes(cmd[i], current_token->token_type);
		i++;
		current_token = current_token->next;
	}
	current_command->command = cmd;
	if (current_token && ft_piperedirect(current_token->token_type) == 1)
	{
		if (current_token->token_type == PIPE_SIGN)
			current_command->pipe = 1;
		if (current_token->token_type == REDIRECT_LEFT)
			current_command->redirect_left = current_token->buffer.len;
		if (current_token->token_type == REDIRECT_RIGHT)
			current_command->redirect_right = current_token->buffer.len;
		current_token = current_token->next;
	}
	if (current_token && ft_piperedirect(current_token->token_type) == 1)
	{
		if (current_token->token_type == PIPE_SIGN)
			current_command->pipe = 1;
		if (current_token->token_type == REDIRECT_LEFT)
			current_command->redirect_left = current_token->buffer.len;
		if (current_token->token_type == REDIRECT_RIGHT)
			current_command->redirect_right = current_token->buffer.len;
		current_token = current_token->next;
	}
}

void	fill_commands(t_vars *vars)
{
	int			i;
	t_token		*current_token;
	t_command	*current_cmd;

	current_token = vars->first;
	i = 0;
	while(current_token)
	{
		if (i == 0)
		{
			vars->cmd = protected_malloc(1, sizeof(t_command));
			current_cmd = vars->cmd;
			current_cmd->next = NULL;
			i++;
		}
		else
		{
			current_cmd = current_cmd->next;
			current_cmd = protected_malloc(1, sizeof(t_command));
			current_cmd->next = NULL;
		}
		fill_command(current_token, current_cmd);
		while (current_token && ft_piperedirect(current_token->token_type) == 0)
			current_token = current_token->next;
		while (current_token && ft_piperedirect(current_token->token_type) == 1)
			current_token = current_token->next;
	}
}

void	parsing(t_vars *vars, char *str)
{
	t_token	*current_token;

	tokenization(vars, str);
	current_token = vars->first;
	while (current_token)
	{
		if (check_error(current_token) == 0)
		{
			if (current_token->token_type == WORD
				|| current_token->token_type == QUOTE)
				replace_env(current_token);
			printf(">>%s<<\n", current_token->buffer.str);
			current_token = current_token->next;
		}
		else
		{
			printf("syntax error near unexpected token\n");
			vars->error = -1;
			break ;
		}
	}
	fill_commands(vars);
	return ;
}
