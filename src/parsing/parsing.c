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

int	check_error2(t_token *token)
{
	t_token *current_token;

	current_token = token;
	if (token->token_type == PIPE_SIGN)
	{
		current_token = current_token->next;
		if (current_token && current_token->token_type == SPACE_SIGN)
			current_token = current_token->next;
		if (current_token && current_token->token_type == PIPE_SIGN)
			return (-1);
	}
	return (0);
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
	if ((token->token_type == REDIRECT_LEFT
			|| token->token_type == REDIRECT_RIGHT)
		&& (token->next == NULL || (token->next->token_type == SPACE_SIGN
				&& token->next->next == NULL)))
		return (-1);
	return (0);
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
	while (current_token)
	{
		if (check_error(current_token) == 0 && check_error2(current_token) == 0)
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
		{
			display_syntax_error(current_token);
			vars->error = -1;
			break ;
		}
	}
	fill_redirect(vars, 0);
	fill_commands(vars, vars->first, 0);
	if (vars->error == 0)
		printf_commands(vars);
}
