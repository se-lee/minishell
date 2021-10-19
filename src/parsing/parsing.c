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

void	add_piperedirect(t_token *current_token, t_command *current_command)
{
	while (current_token && ft_piperedirect(current_token->token_type) == 1)
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

int	find_space(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			while (str[i] == ' ')
				i++;
			if (str[i] && str[i != ' '])
				j++;
		}
		i++;
	}
	return (j);
}

char	**command_create(t_token *current_token, int i)
{
	char	**cmd;
	char	**split_str;
	int		j;

	cmd = protected_malloc(i + 1, sizeof(char *));
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		j = find_space(current_token->buffer.str);
		if (j != 0 && current_token->token_type == WORD)
		{
			split_str = ft_split(current_token->buffer.str, ' ');
			j = 0;
			while (split_str[j])
			{
				cmd[i] = ft_strdup(split_str[j]);
				j++;
				i++;
			}
			free_array(split_str);
		}
		else
			cmd[i] = ft_strdup(current_token->buffer.str);
		cmd[i] = remove_quotes(cmd[i], current_token->token_type);
		i++;
		current_token = current_token->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

// void	fill_command(t_token *token, t_command *current_command)
// {
// 	int		i;
// 	t_token	*current_token;
// 	char	**cmd;

// 	current_command->pipe = 0;
// 	current_command->redirect_left = 0;
// 	current_command->redirect_right = 0;
// 	i = 0;
// 	current_token = token;
// 	while (current_token && ft_piperedirect(current_token->token_type) == 0)
// 	{
// 		if (current_token->token_type == WORD)
// 			i += find_space(current_token->buffer.str);
// 		i++;
// 		current_token = current_token->next;
// 	}
// 	current_token = token;
// 	cmd = command_create(current_token, i);
// 	current_command->command = cmd; 
// 	add_piperedirect(current_token, current_command);
// }

void	fill_command(t_token *token, t_command *current_command)
{
	int		i;
	char	**cmd;
	t_token	*current_token;

	current_command->pipe = 0;
	current_command->redirect_left = 0;
	current_command->redirect_right = 0;
	current_command->quotes = 0; // ADD
	i = 0;
	current_token = token;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		if (ft_piperedirect(current_token->token_type) == 0)
			i += find_space(current_token->buffer.str);
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
		if (current_token->token_type == QUOTE || current_token->token_type == SINGLE_QUOTE)
			current_command->quotes = 1;
		current_token = current_token->next;
	}
	cmd[i] = NULL;
	current_command->command = cmd;
	current_token = token;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
		current_token = current_token->next;
	add_piperedirect(current_token, current_command);
}

void	fill_commands(t_vars *vars)
{
	int			i;
	t_token		*current_token;
	t_command	*current_cmd;

	current_token = vars->first;
	i = 0;
	while (current_token)
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
			current_cmd->next = protected_malloc(1, sizeof(t_command));
			current_cmd = current_cmd->next;
			current_cmd->next = NULL;
		}
		fill_command(current_token, current_cmd);
		while (current_token && ft_piperedirect(current_token->token_type) == 0)
			current_token = current_token->next;
		while (current_token && ft_piperedirect(current_token->token_type) == 1)
			current_token = current_token->next;
	}
}

void	printf_commands(t_vars *vars)
{
	int			i;
	t_command	*current_cmd;

	current_cmd = vars->cmd;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->command[i])
		{
			printf("command[%d] = %s\n", i, current_cmd->command[i]);
			i++;
		}
		current_cmd = current_cmd->next;
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
				replace_env(vars, current_token);
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
	// printf_commands(vars);
	return ;
}
