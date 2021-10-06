#include "minishell.h"

int	format_is_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	ft_inenv(t_envlist *envp, char *str)
{
	t_envlist	*current_env;
	char		*name;

	current_env = envp;
	name = env_separation(str, 0);
	while (current_env && (ft_strncmp(current_env->name,
				name, (ft_strlen(name) + 1)) != 0))
		current_env = current_env->next;
	if (current_env)
		return (1);
	return (0);
}

int	export_syntax(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]) != 1 && (str[i] != '-') && (str[i] != '_'))
			return (0);
		i++;
	}
	return (1);
}
