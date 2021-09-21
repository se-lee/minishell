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
	int i;

	i = 0;
	while (ft_isspecial(str[i]) == 0)
	{
		token->token_type = word;
		add_char(token, str[i]);
		i++;
	}
	return (i + 1);
}

char	**tokenization(t_vars *vars, char *str)
{
	int			i;
	int			j;
	t_command	command;

	command = vars->first;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (isspecial(str[i]) == 0)
		{
			i += token_word(command.token, &str[i]);
			command = command.next;
		}
		j++;
	}
}

void parsing(t_vars *vars, char *str)
{
	char	**tokens;
	tokens = tokenization(vars, str);
	return ;
}
