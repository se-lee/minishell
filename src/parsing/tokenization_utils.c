#include "minishell.h"

void	add_char(t_token *token, char c)
{
	char	*str;

	token->buffer.len++;
	str = ft_strjoin_char(token->buffer.str, c);
	free(token->buffer.str);
	token->buffer.str = str;
}

void	update_token(t_token *token, char *var, char *value)
{
	char	*string;

	string = replace(token->buffer.str, var, value);
	free(var);
	free(token->buffer.str);
	token->buffer.str = string;
	token->buffer.len = ft_strlen(token->buffer.str);
}
