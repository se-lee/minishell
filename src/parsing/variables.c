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

void	token_cut(t_vars *vars, t_token *token)
{
	t_token	*current_token;

	current_token = vars->first;
	while (current_token->next && current_token->next != token)
	{
		printf("current_token = %p, token = %p\n", current_token->next, token);
		current_token = current_token->next;
	}
	printf("FOUND IT\n");
		printf("current_token = %p, token = %p\n", current_token->next, token);
}

void	replace_env(t_vars *vars, t_token *token)
{
	int		i;
	char	*var;
	char	*value;

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
	token_cut(vars, token);
}
