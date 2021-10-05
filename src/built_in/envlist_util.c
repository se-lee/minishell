#include "minishell.h"

char	*env_separation(char *str, int part)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (part == 0)
	{
		new_str = protected_malloc(i + 1, sizeof(char));
		while (j < i)
		{
			new_str[j] = str[j];
			j++;
		}
		new_str[j] = '\0';
		return (new_str);
	}
	if (str[i] && part == 1)
	{
		new_str = protected_malloc(ft_strlen(str) - i + 1, sizeof(char));
		while (i < ft_strlen(str))
		{
			new_str[j] = str[i + 1];
			j++;
			i++;
		}
		new_str[j] = '\0';
		return (new_str);
	}
	return (NULL);
}

void	envlist_create(t_vars *vars, char **envp)
{
	int			i;
	t_envlist	*current_envlist;

	vars->envp = malloc(sizeof(t_envlist));
	current_envlist = vars->envp;
	current_envlist->name = env_separation(envp[0], 0);
	current_envlist->value = env_separation(envp[0], 1);
	current_envlist->next = NULL;
	i = 1;
	while (envp[i])
	{
		current_envlist->next = malloc(sizeof(t_envlist));
		current_envlist = current_envlist->next;
		current_envlist->name = env_separation(envp[i], 0);
		current_envlist->value = env_separation(envp[i], 1);
		current_envlist->next = NULL;
		i++;
	}
	current_envlist = vars->envp;
	while (current_envlist && ft_strncmp(current_envlist->name,
			"OLDPWD", 7) != 0)
		current_envlist = current_envlist->next;
	if (current_envlist && ft_strncmp(current_envlist->name,
			"OLDPWD", 7) == 0)
	{
		free(current_envlist->value);
		current_envlist->value = NULL;
	}
}

void	envlist_free(t_envlist *to_free)
{
	t_envlist	*current_env;
	t_envlist	*temp;

	current_env = to_free;
	while (current_env->next)
	{
		temp = current_env->next;
		free(current_env->name);
		if (current_env->value)
			free(current_env->value);
		free(current_env);
		current_env = temp;
	}
	if (current_env)
		free(current_env);
}

void	envlist_print_all(t_envlist *envp)
{
	t_envlist	*current_env;

	current_env = envp;
	while (current_env->next != NULL)
	{
		if (current_env->value)
		{
			printf("%s=", current_env->name);
			if (ft_strncmp(current_env->value, "", 2) == 0)
				printf("\"\"\n");
			else
				printf("%s\n", current_env->value);
		}
		else
			printf("%s\n", current_env->name);
		current_env = current_env->next;
	}
	if (current_env->value)
	{
		printf("%s=", current_env->name);
		printf("%s\n", current_env->value);
	}
}

t_envlist	*envlist_duplicate(t_envlist *envp)
{
	t_envlist	*new_env;
	t_envlist	*current_env;
	char		*temp;
	t_envlist	*first;

	current_env = envp;
	new_env = protected_malloc(1, sizeof(t_envlist));
	temp = ft_strdup(current_env->name);
	new_env->name = ft_strjoin("declare -x ", temp);
	free(temp);
	if (current_env->value != NULL)
	{
		new_env->value = ft_strdup(current_env->value);
	}
	else
		new_env->value = NULL;
	new_env->next = NULL;
	first = new_env;
	current_env = current_env->next;
	while (current_env)
	{
		new_env->next = protected_malloc(1, sizeof(t_envlist));
		new_env = new_env->next;
		temp = ft_strdup(current_env->name);
		new_env->name = ft_strjoin("declare -x ", temp);
		free(temp);
		if (current_env->value != NULL)
			new_env->value = ft_strdup(current_env->value);
		else
			new_env->value = NULL;
		new_env->next = NULL;
		current_env = current_env->next;
	}
	return (first);
}
