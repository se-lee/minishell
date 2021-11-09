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
