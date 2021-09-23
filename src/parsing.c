#include "minishell.h"

void	add_char(t_token *token, char c)
{
	char *str;

	token->buffer.len++;
	str = ft_strjoin_char(token->buffer.str, c);
	free(token->buffer.str);
	token->buffer.str = str;
}

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
	while (j < ft_strlen(real))
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
	while (ft_isupper(str[i]) == TRUE)
		i++;
	var = protected_malloc((i + 1), sizeof(char));
	i = 0;
	while (ft_isupper(str[i]) == TRUE)
	{
		var[i] = str[i];
		i++;
	}
	return (var);
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

void parsing(t_vars *vars, char *str)
{
	// char	**tokens;

	tokenization(vars, str);
	return ;
}
