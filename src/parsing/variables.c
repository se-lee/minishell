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

void	replace_env(t_token *token)
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
			value = getenv(&var[1]);
			update_token(token, var, value);
			i = -1;
		}
		i++;
	}
}