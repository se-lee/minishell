#include "minishell.h"

int	isspecial(char c)
{
	if (c == '"' || c == '\'' || c == '$'
		|| c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

void	add_char(t_token *token, char c)
{
	char *str;

	token->buffer->len++;
	str = ft_strjoin_char(token->buffer->str, c);
	free(token->buffer->str);
	token->buffer->str = str;
}

int	token_word(t_token *token, char *str)
{
	int		i;
	char	*string;

	token->token_type = word;
	string = malloc(sizeof(char) * 2);
	string[0] = str[0];
	string[1] = '\0';
	token->buffer->len = 1;
	dprintf(1, "yo1\n");
	token->buffer->str = ft_strdup(string);
	free(string);
	dprintf(1, "yo2\n");
	i = 1;
	while (isspecial(str[i]) == 0)
	{
		add_char(token, str[i]);
		i++;
	}
	return (i + 1);
}

int	token(t_token *token, char *str, char c, enum TYPE token_type)
{
	int i;

	i = 0;
	token->token_type = token_type;
	while (str[i] == c)
	{
		add_char(token, str[i]);
		i++;
	}
	return (i + 1);
}

int	token_quote(t_token *token, char *str, char c, enum TYPE token_type)
{
	int i;

	token->token_type = token_type;
	add_char(token, str[0]);
	i = 1;
	while (str[i] != 'c')
	{
		add_char(token, str[i]);
		i++;
	}
	add_char(token, str[i]);
	i++;
	return (i + 1);
}

void	tokenization(t_vars *vars, char *str)
{
	int			i;
	t_command	command;

	command = vars->first;
	i = 0;
	while (str[i])
	{
		if (isspecial(str[i]) == 0)
			i += token_word(&command.token, &str[i]);
		else if (str[i] == '"')
			i += token_quote(&(command.token), &str[i], '"', quote);
		else if (str[i] == '\'')
			i += token_quote(&(command.token), &str[i], '\'', single_quote);
		else if (str[i] == ' ')
			i += token(&(command.token), &str[i], ' ', space);
		else if (str[i] == '|')
			i += token(&(command.token), &str[i], '|', pipe_sign);
		printf("%s\n", command.token.buffer->str);
		command = *(command.next);
	}
}

void parsing(t_vars *vars, char *str)
{
	// char	**tokens;

	tokenization(vars, str);
	return ;
}
