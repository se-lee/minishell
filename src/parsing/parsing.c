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
	if (token->token_type == PIPE_SIGN && token->buffer.len > 1)
		return (-1);
	if ((token->token_type == QUOTE || token->token_type == SINGLE_QUOTE)
		&& token->quote_nb != 2)
		return (-1);
	if (ft_piperedirect(token->token_type) == 1 && token->buffer.len > 2)
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

/*

*/
char	**command_create(t_token *current_token, int i)
{
	char	**cmd;
	char	**split_str;
	int		j;

	cmd = protected_malloc(i + 1, sizeof(char *));
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		cmd[i] = ft_strdup(current_token->buffer.str);
		cmd[i] = remove_quotes(cmd[i], current_token->token_type);
		i++;
		current_token = current_token->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

int	array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
		i++;
	return (i);
}

char	**array_realloc(char **array, char *new_line)
{
	char	**new_array;
	int		i;

	i = 0;
	new_array = protected_malloc(array_len(array) + 2, sizeof(char *));
	while (array[i] != NULL)
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = ft_strdup(new_line);
	new_array[i + 1] = NULL;
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return(new_array);
}

void	initialize_command(t_command *command)
{
	command->pipe = 0;
	command->redirect_left = 0;
	command->redirect_right = 0;
	command->quotes = 0;
}

void	fill_command(t_token *token, t_command *current_command)
{
	int		i;
	char	**cmd;
	char	*temp;
	t_token	*current_token;

	initialize_command(current_command);
	current_token = token;
	if (current_token && current_token->token_type == SPACE)
		current_token = current_token->next;
	cmd = protected_malloc(2, sizeof(char *));
	current_token->buffer.str = remove_quotes(current_token->buffer.str, current_token->token_type);
	cmd[0] = ft_strdup(current_token->buffer.str);
	cmd[1] = NULL;
	current_token = current_token->next;
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		// printf(">>%s<<\n", current_token->buffer.str);
		// printf("out_cmd[%d] = >>%s<<\n", i, cmd[i]);
		if (current_token->token_type != SPACE)
		{
			while (current_token && ft_piperedirect(current_token->token_type) == 0
				&& current_token->token_type != SPACE)
			{
				// printf("in_cmd[%d] = >>%s<<\n", i, cmd[i]);
				if (cmd[i] != NULL)
				{
					// printf("if_cmd[%d] = >>%s<<\n", i, cmd[i]);
					current_token->buffer.str = remove_quotes(current_token->buffer.str, current_token->token_type);
					temp = ft_strjoin(cmd[i], current_token->buffer.str);
					free(cmd[i]);
					cmd[i] = temp;
					cmd[i + 1] = NULL;
				}
				else
				{
					// printf("else_cmd[%d] = >>%s<<\n", i, cmd[i]);
					current_token->buffer.str = remove_quotes(current_token->buffer.str, current_token->token_type);
					cmd = array_realloc(cmd, current_token->buffer.str);
				}
				current_token = current_token->next;
			}
		}
		if (current_token && current_token->token_type == SPACE)
		{
			// printf("increment i:%d\n", i);
			i++;
		}
		if (current_token && ft_piperedirect(current_token->token_type) == 0)
			current_token = current_token->next;
	}
	if (current_token)
		add_piperedirect(current_token, current_command);
	current_command->command = cmd;
}

void	fill_commands(t_vars *vars)
{
	int			i;
	t_token		*current_token;
	t_command	*current_cmd;

	// current_token = vars->first;
	// while (current_token)
	// {
	// 	printf("token:>>%s<<\n", current_token->buffer.str);
	// 	current_token = current_token->next;
	// }
	current_token = vars->first;
	i = 0;
	while (current_token)
	{
		if (current_token->token_type == SPACE && current_token->next)
		{
			current_token = current_token->next;
			while (current_token && ft_piperedirect(current_token->token_type) == 1)
				current_token = current_token->next;
		}
		if (current_token->token_type != SPACE)
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

int	check_error2(t_token *token)
{
	t_token	*current_token;
	int		word;
	int		pipe;
	int		redirect;

	word = 0;
	pipe = 0;
	redirect = 0;
	current_token = token;
	while (current_token)
	{
		if (current_token->token_type == SPACE)
			current_token = current_token->next;
		if (ft_piperedirect(current_token->token_type) == 0)
		{
			word++;
			pipe = 0;
		}
		if (current_token->token_type == PIPE_SIGN)
			pipe++;
		if (current_token->token_type == REDIRECT_LEFT
			|| current_token->token_type == REDIRECT_RIGHT)
			redirect++;
		if (redirect == 1)
		{
			if (current_token->token_type == SPACE)
				current_token = current_token->next;
			if (ft_piperedirect(current_token->token_type) != 0)
				return (-1);
			redirect = 0;
		}
		if ((pipe == 1 && word == 0) || (pipe > 1))
			return (-1);
		current_token = current_token->next;
	}
	return (0);
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

t_token	*fill_inout(t_vars *vars, t_token *current_token, t_redirect *current_inout, int cmd_num)
{
	current_inout->arrow_num = current_token->buffer.len;
	current_inout->cmd_num = cmd_num;
	current_token = remove_token(vars, current_token);
	if (current_token->token_type == SPACE)
		current_token = remove_token(vars, current_token);
	current_inout->filename = ft_strdup(current_token->buffer.str);
	current_token = remove_token(vars, current_token);
	return (current_token);
}

void	fill_redirect(t_vars *vars)
{
	t_token		*current_token;
	t_redirect	*current_in;
	t_redirect	*current_out;
	int			cmd_num;

	current_token = vars->first;
	vars->in = NULL;
	vars->out = NULL;
	cmd_num = 0;
	while (current_token)
	{
		if (current_token->token_type == PIPE_SIGN)
			cmd_num++;
		if (current_token->token_type == REDIRECT_LEFT)
		{
			if (vars->in == NULL)
			{
				vars->in = protected_malloc(1, sizeof(t_redirect));
				current_in = vars->in;
				current_in->next = NULL;
			}
			else
			{
				current_in->next = protected_malloc(1, sizeof(t_redirect));
				current_in = current_in->next;
				current_in->next = NULL;
			}
			current_token = fill_inout(vars, current_token, current_in, cmd_num);
		}
		else if (current_token->token_type == REDIRECT_RIGHT)
		{
			if (vars->out == NULL)
			{
				vars->out = protected_malloc(1, sizeof(t_redirect));
				current_out = vars->out;
				current_out->next = NULL;
			}
			else
			{
				current_out->next = protected_malloc(1, sizeof(t_redirect));
				current_out = current_out->next;
				current_out->next = NULL;
			}
			current_token = fill_inout(vars, current_token, current_out, cmd_num);
		}
		else
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
		if (check_error(current_token) == 0)// && check_error2(vars->first) == 0)
		{
			if (current_token->token_type == WORD
				|| current_token->token_type == QUOTE)
				current_token = replace_env(vars, current_token);
			else
				current_token = current_token->next;
		}
		else
		{
			printf("syntax error near unexpected token\n");
			vars->error = -1;
			break ;
		}
	}
	fill_redirect(vars);
	fill_commands(vars);
	if (vars->error == 0)
	{
		printf_commands(vars);
	}
	return ;
}
