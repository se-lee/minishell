#include "minishell.h"

char	*find_variable(char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '"')
		i++;
	var = protected_malloc((i + 1), sizeof(char));
	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '"')
	{
		var[i] = str[i];
		i++;
	}
	var[i] = '\0';
	return (var);
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
		current_token->next = protected_malloc(1, sizeof(t_token));
		current_token = current_token->next;
		current_token->next = NULL;
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
	k = 0;
	temp_next = token->next;
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
		current_token->next = protected_malloc(1, sizeof(t_token));
		current_token = current_token->next;
		current_token->next = NULL;
	}
	while (token->buffer.str[i] && token->buffer.str[i] == ' ' && token->token_type == WORD)
		i++;
	//	if j != i -> need to add a token in the middle with token_type = SPACE_SIGN and len = i - j
	j = i;
	while (token->buffer.str[i])
	{
		while (token->buffer.str[i] && token->buffer.str[i] != ' ')
			i++;
		if (k != 0)
		{
			current_token->next = protected_malloc(1, sizeof(t_token));
			current_token = current_token->next;
			current_token->next = NULL;
		}
		current_token->buffer.str = ft_strndup(&token->buffer.str[j], i - j);
		current_token->token_type = token->token_type;
		current_token->buffer.len = ft_strlen(current_token->buffer.str);
		while (token->buffer.str[i] && token->buffer.str[i] == ' ')
			i++;
		k++;
		j = i;
	}
	current_token->next = temp_next;
	free(token->buffer.str);
	free(token);
	return (temp_next);
}

t_token *replace_env(t_vars *vars, t_token *token)
{
	int		i;
	char	*var;
	char	*value;
	t_token	*current_token;

	i = 0;
	while (token->buffer.str[i])
	{
		if (token->buffer.str[i] == '$')
		{
			var = find_variable(&token->buffer.str[i]);
			value = get_env_value(vars->envp, &var[1]);
			update_token(token, var, value);
			i = -1;
		}
		i++;
	}
	if (token->buffer.str && find_space(token->buffer.str) != 0)
	{
		//	printf("%s\n", token->buffer.str);
		if (token->token_type == WORD)
		{
			token = token_hell(vars, token);
			return (token);
		}
	}
	return (token->next);
}
