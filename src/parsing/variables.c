#include "minishell.h"

char	*find_variable(char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'')
		i++;
	var = protected_malloc((i + 1), sizeof(char));
	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'')
	{
		var[i] = str[i];
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

t_token	*token_malloc_first(t_vars *vars, t_token *token)
{
	t_token	*current_token;

	current_token = vars->first;
	if (token == vars->first)
	{
		vars->first = NULL;
		vars->first = protected_malloc(1, sizeof(t_token));
		current_token = vars->first;
		current_token->next = NULL;
	}
	else
	{
		while (current_token->next && current_token->next != token)
			current_token = current_token->next;
		current_token->next = NULL;
		malloc_token_next(&current_token);
	}
	return (current_token);
}

t_token	*token_hell(t_vars *vars, t_token *token)
{
	t_token	*current_token;
	t_token	*temp_next;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	temp_next = token->next;
	while (token->buffer.str[i] && token->buffer.str[i] == ' '
		&& token->token_type == WORD)
		i++;
	while (token->buffer.str[i])
	{
		if (k == 0)
			current_token = token_malloc_first(vars, token);
		else
			malloc_token_next(&current_token);
		k++;
		while (token->buffer.str[i] && token->buffer.str[i] != ' ')
			i++;
		current_token->buffer.str = ft_strndup(&token->buffer.str[j], i - j);
		current_token->token_type = token->token_type;
		current_token->buffer.len = ft_strlen(current_token->buffer.str);
		j = i;
		while (token->buffer.str[i] && token->buffer.str[i] == ' '
			&& token->token_type == WORD)
			i++;
		if (j != i && token->buffer.str[i])
		{
			malloc_token_next(&current_token);
			current_token->token_type = SPACE_SIGN;
			current_token->buffer.len = i - j;
			current_token->buffer.str = ft_strndup(&token->buffer.str[j],
					i - j);
		}
		j = i;
	}
	current_token->next = temp_next;
	free(token->buffer.str);
	free(token);
	return (temp_next);
}

t_token	*replace_env(t_vars *vars, t_token *token)
{
	int		i;
	char	*var;
	char	*value;

	i = -1;
	while (token->buffer.str[++i])
	{
		if (token->buffer.str[i] == '$')
		{
			var = find_variable(&token->buffer.str[i]);
			value = get_env_value(vars->envp, &var[1], vars->return_value);
			update_token(token, var, value);
			i = -1;
		}
	}
	if (token->buffer.str && find_space(token->buffer.str) != 0)
	{
		if (token->token_type == WORD)
		{
			token = token_hell(vars, token);
			return (token);
		}
	}
	return (token->next);
}
