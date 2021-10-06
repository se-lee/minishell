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

char    *get_env_value(t_envlist *envp, char *env_name)
{
    t_envlist *current_env;
    char    *value;

    current_env = envp;
    value = NULL;
    while (current_env)
    {
        if (ft_strcmp(current_env->name, env_name) == 0 && current_env->value)
            value = ft_strdup(current_env->value);
        current_env = current_env->next;    
    }
    return (value);
}

void	replace_env(t_envlist *envp, t_token *token)
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
			value = get_env_value(envp, &var[1]);
			update_token(token, var, value);
			i = -1;
		}
		i++;
	}
}
