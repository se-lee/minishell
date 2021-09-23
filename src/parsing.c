#include "minishell.h"

char	*replace(char *full, char *placeholder, char *real)
{
	int		i;
	int		j;
	int		len;
	char	*final;

	len = ft_strlen(full) + ft_strlen(real) - ft_strlen(placeholder);
	final = protected_malloc((len + 1), sizeof(char));
	i = 0;
	while (ft_strncmp(&full[i], placeholder, ft_strlen(placeholder)) != 0)
	{
		final[i] = full[i];
		i++;
	}
	j = 0;
	while (j < (int)ft_strlen(real))
	{
		final[i + j] = real[j];
		j++;
	}
	j = i + j;
	i += ft_strlen(placeholder);
	while (j < len)
		final[j++] = full[i++];
	final[j] = '\0';
	return (final);
}

char	*find_variable(char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	var = protected_malloc((i + 1), sizeof(char));
	i = 0;
	while (str[i] && str[i] != ' ')
	{
		var[i] = str[i];
		i++;
	}
	var[i] = '\0';
	return (var);
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

void parsing(t_vars *vars, char *str)
{
	t_token	*current_token;

	tokenization(vars, str);
	current_token = vars->first;
	while (current_token)
	{
		if (current_token->token_type != SINGLE_QUOTE)
			replace_env(current_token);
		printf("%s\n", current_token->buffer.str);
		current_token = current_token->next;
	}
	return ;
}
