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
	string = malloc(sizeof(char) * 2);
	string[0] = str[0];
	string[1] = '\0';
	token->buffer.str = string;
	token->buffer.len = 1;
	i = 1;
	while (str[i] && isspecial(str[i]) == FALSE)
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

	// printf("yo\n");
	token->token_type = token_type;
	string = malloc(sizeof(char) * 2);
	string[0] = str[0];
	string[1] = '\0';
	// printf("%s\n", string);
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

char	*find_variable(char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (ft_isupper(str[i]) == TRUE)
		i++;
	var = malloc(sizeof(char) * (i + 1));
	i = 0;
	while (ft_isupper(str[i]) == TRUE)
	{
		var[i] = str[i];
		i++;
	}
	return (var);
}

char	*replace(char *full, char *placeholder, char *real)
{
	int		i;
	int		j;
	int		len;
	char	*final;

	len = ft_strlen(full) - ft_strlen(placeholder) + ft_strlen(real);
	final = malloc(sizeof(char) * (len + 1));
	i = 0;
	while (ft_strncmp(full, placeholder, ft_strlen(placeholder)) != 0)
	{
		dprintf(1, "yo\n");
		final[i] = full[i];
		i++;
	}
	j = 0;
	while (j < ft_strlen(real))
	{
		final[i + j] = real[j];
		j++;
	}
	i += ft_strlen(placeholder);
	j = i + j;
	while (i < len)
	{
		final[j] = full[i];
		i++;
		j++;
	}
	final[i] = '\0';
	return (final);
}

void	replace_env(char *str)
{
	int		i;
	int		j;
	char	*var;
	char	*value;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && ft_isupper(str[i + 1]) == TRUE)
		{
			var = find_variable(&str[i]);
			value = getenv(&var[1]);
			if (ft_strncmp(value, "(null)", 6))
				str = replace(str, var, NULL);
			else
				str = replace(str, var, value);
			i = -1;
		}
		i++;
	}
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
	string = malloc(sizeof(char) * 2);
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
	replace_env(token->buffer.str);
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
			vars->first = malloc(sizeof(t_token));
			current_token = vars->first;
		}
		else
		{
			current_token->next = NULL;
			current_token = current_token->next;
			current_token = malloc(sizeof(t_token));
		}
		if (isspecial(str[i]) == FALSE)
			i += token_word(current_token, &str[i]);
		else if (str[i] == ' ')
			i += token(current_token, &str[i], ' ', SPACE);
		else if (str[i] == '|')
			i += token(current_token, &str[i], '|', PIPE_SIGN);
		else if (str[i] == '"')
			i += token_quote(current_token, &str[i], QUOTE);
		else if (str[i] == '\'')
			i += token_quote(current_token, &str[i], SINGLE_QUOTE);
		printf("%s\n", current_token->buffer.str);
	}
}

void parsing(t_vars *vars, char *str)
{
	// char	**tokens;

	tokenization(vars, str);
	return ;
}
