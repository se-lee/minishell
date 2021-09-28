#include "minishell.h"

int	token_word(t_token *token, char *str)
{
	int		i;
	char	*string;

	token->token_type = WORD;
	string = protected_malloc((2), sizeof(char));
	string[0] = str[0];
	string[1] = '\0';
	token->buffer.str = string;
	token->buffer.len = 1;
	i = 1;
	while (str[i] && ft_isspecial(str[i]) == FALSE)
	{
		add_char(token, str[i]);
		i++;
	}
	return (i);
}

int	token(t_token *token, char *str, char c, enum e_type token_type)
{
	int		i;
	char	*string;

	token->token_type = token_type;
	string = protected_malloc((2), sizeof(char));
	string[0] = str[0];
	string[1] = '\0';
	token->buffer.str = string;
	token->buffer.len = 1;
	i = 1;
	while (str[i] && str[i] == c)
	{
		add_char(token, str[i]);
		i++;
	}
	return (i);
}

int	token_quote(t_token *token, char *str, char c, enum e_type token_type)
{
	int		i;
	char	*string;

	token->token_type = token_type;
	string = protected_malloc((2), sizeof(char));
	string[0] = str[0];
	string[1] = '\0';
	token->buffer.str = string;
	token->buffer.len = 1;
	token->quote_nb = 1;
	i = 1;
	while (str[i] && str[i] != c)
	{
		add_char(token, str[i]);
		i++;
	}
	if (str[i])
	{
		token->quote_nb++;
		add_char(token, str[i]);
		i++;
	}
	return (i);
}

int	token_identification(t_token *current_token, char *str)
{
	int	i;

	i = 0;
	if (ft_isspecial(str[0]) == FALSE)
		i += token_word(current_token, str);
	else if (str[0] == ' ')
		i += token(current_token, str, ' ', SPACE_SIGN);
	else if (str[0] == '|')
		i += token(current_token, str, '|', PIPE_SIGN);
	else if (str[0] == '>')
		i += token(current_token, str, '>', REDIRECT);
	else if (str[0] == '<')
		i += token(current_token, str, '<', REDIRECT);
	else if (str[0] == '"')
		i += token_quote(current_token, str, '"', QUOTE);
	else if (str[0] == '\'')
		i += token_quote(current_token, str, '\'', SINGLE_QUOTE);
	return (i);
}

void	tokenization(t_vars *vars, char *str)
{
	int			i;
	t_token		*current_token;

	i = 0;
	while (str[i])
	{
		if (i == 0)
		{
			vars->first = protected_malloc(1, sizeof(t_token));
			current_token = vars->first;
			current_token->next = NULL;
		}
		else
		{
			current_token->next = protected_malloc(1, sizeof(t_token));
			current_token = current_token->next;
			current_token->next = NULL;
		}
		i += token_identification(current_token, &str[i]);
	}
}
