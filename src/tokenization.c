#include "minishell.h"

void	add_char(t_token *token, char c)
{
	char *str;

	token->buffer.len++;
	str = ft_strjoin_char(token->buffer.str, c);
	free(token->buffer.str);
	token->buffer.str = str;
}

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
	int 	i;
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

int	token_quote(t_token *token, char *str, enum e_type token_type)
{
	int		i;
	char	*string;
	char	c;

	token->token_type = token_type;
	if (token_type == QUOTE)
		c = '"';
	else
		c = '\'';
	string = protected_malloc((2), sizeof(char));
	string[0] = str[0];
	string[1] = '\0';
	i = 1;
	while (str[i] && str[i] != 'c')
	{
		add_char(token, str[i]);
		i++;
	}
	add_char(token, str[i]);
	i++;
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
		if (ft_isspecial(str[i]) == FALSE)
			i += token_word(current_token, &str[i]);
		else if (str[i] == ' ')
			i += token(current_token, &str[i], ' ', SPACE_SIGN);
		else if (str[i] == '|')
			i += token(current_token, &str[i], '|', PIPE_SIGN);
		else if (str[i] == '>')
			i += token(current_token, &str[i], '>', REDIRECT);
		else if (str[i] == '<')
			i += token(current_token, &str[i], '<', REDIRECT);
		else if (str[i] == '"')
			i += token_quote(current_token, &str[i], QUOTE);
		else if (str[i] == '\'')
			i += token_quote(current_token, &str[i], SINGLE_QUOTE);
	}
}
